package com.wechat.backend;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.wechat.backend.entity.User;
import com.wechat.backend.repository.UserRepository;
import com.wechat.backend.security.JwtService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;

import java.time.Duration;
import java.util.Map;

import static org.hamcrest.Matchers.containsString;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@SpringBootTest
@AutoConfigureMockMvc
@ActiveProfiles("test")
class BackendIntegrationTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private ObjectMapper objectMapper;

    @Autowired
    private UserRepository userRepository;

    @Autowired
    private PasswordEncoder passwordEncoder;

    @Autowired
    private JwtService jwtService;

    @Test
    void healthEndpointShouldBePublic() throws Exception {
        mockMvc.perform(get("/api/health"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.status").value("UP"));
    }

    @Test
    void registerShouldSucceedAndDuplicateShouldReturn409() throws Exception {
        Map<String, Object> payload = Map.of(
                "phone", "13800000001",
                "password", "password123"
        );

        mockMvc.perform(post("/api/auth/register")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(payload)))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.userId").isNumber());

        mockMvc.perform(post("/api/auth/register")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(payload)))
                .andExpect(status().isConflict())
                .andExpect(jsonPath("$.message").value("Phone already registered"));
    }

    @Test
    void loginShouldSucceedWithCorrectPasswordAndFailWithWrongPassword() throws Exception {
        register("13800000002", "password123");

        mockMvc.perform(post("/api/auth/login")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(Map.of("phone", "13800000002", "password", "password123"))))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.token").isString());

        mockMvc.perform(post("/api/auth/login")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(Map.of("phone", "13800000002", "password", "wrong123"))))
                .andExpect(status().isUnauthorized())
                .andExpect(jsonPath("$.message").value("Password is incorrect"));
    }

    @Test
    void loginShouldReturn404WhenAccountDoesNotExist() throws Exception {
        mockMvc.perform(post("/api/auth/login")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(Map.of("phone", "13800000099", "password", "password123"))))
                .andExpect(status().isNotFound())
                .andExpect(jsonPath("$.message").value("Account does not exist"));
    }

    @Test
    void registerShouldReturn400WhenPasswordTooShort() throws Exception {
        mockMvc.perform(post("/api/auth/register")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(Map.of("phone", "13800000005", "password", "123"))))
                .andExpect(status().isBadRequest())
                .andExpect(jsonPath("$.message", containsString("password")));
    }

    @Test
    void sensorWriteShouldRequireTokenAndReadShouldBePublic() throws Exception {
        register("13800000003", "password123");
        String token = loginAndGetToken("13800000003", "password123");

        Map<String, Object> sensorPayload = Map.of(
                "topic", "zigbee",
                "temperature", "25",
                "humidity", "50",
                "air", "100",
                "brightness", "88",
                "bemfaTime", "2026-03-24 12:00:00",
                "rawPayload", "#25#50#100#88"
        );

        mockMvc.perform(post("/api/logs/sensors")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(sensorPayload)))
                .andExpect(status().isUnauthorized());

        mockMvc.perform(post("/api/logs/sensors")
                        .contentType(MediaType.APPLICATION_JSON)
                        .header("Authorization", "Bearer " + token)
                        .content(toJson(sensorPayload)))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.id").isNumber());

        mockMvc.perform(get("/api/logs/sensors"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.content[0].topic").value("zigbee"));
    }

    @Test
    void expiredTokenShouldBeRejected() throws Exception {
        User user = new User();
        user.setPhone("13800000004");
        user.setPasswordHash(passwordEncoder.encode("password123"));
        user.touchForCreate();
        user = userRepository.save(user);

        String expiredToken = jwtService.generateToken(user.getId(), user.getPhone(), Duration.ofSeconds(-5));

        Map<String, Object> operationPayload = Map.of(
                "topic", "light002",
                "commandKey", "key1",
                "commandLabel", "Light Toggle",
                "result", "FAIL",
                "bemfaResponse", "timeout"
        );

        mockMvc.perform(post("/api/logs/operations")
                        .contentType(MediaType.APPLICATION_JSON)
                        .header("Authorization", "Bearer " + expiredToken)
                        .content(toJson(operationPayload)))
                .andExpect(status().isUnauthorized());
    }

    @Test
    void operationWriteShouldRequireTokenAndReadShouldBePublic() throws Exception {
        register("13800000006", "password123");
        String token = loginAndGetToken("13800000006", "password123");

        Map<String, Object> operationPayload = Map.of(
                "topic", "light002",
                "commandKey", "key2",
                "commandLabel", "Fan Toggle",
                "result", "SUCCESS",
                "bemfaResponse", "{\"status\":\"ok\"}"
        );

        mockMvc.perform(post("/api/logs/operations")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(operationPayload)))
                .andExpect(status().isUnauthorized());

        mockMvc.perform(post("/api/logs/operations")
                        .contentType(MediaType.APPLICATION_JSON)
                        .header("Authorization", "Bearer " + token)
                        .content(toJson(operationPayload)))
                .andExpect(status().isCreated())
                .andExpect(jsonPath("$.id").isNumber());

        mockMvc.perform(get("/api/logs/operations"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.content[0].commandKey").value("key2"));
    }

    private void register(String phone, String password) throws Exception {
        mockMvc.perform(post("/api/auth/register")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(Map.of("phone", phone, "password", password))))
                .andExpect(status().isCreated());
    }

    private String loginAndGetToken(String phone, String password) throws Exception {
        MvcResult result = mockMvc.perform(post("/api/auth/login")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(toJson(Map.of("phone", phone, "password", password))))
                .andExpect(status().isOk())
                .andReturn();

        JsonNode node = objectMapper.readTree(result.getResponse().getContentAsString());
        return node.path("token").asText();
    }

    private String toJson(Object obj) throws Exception {
        return objectMapper.writeValueAsString(obj);
    }
}
