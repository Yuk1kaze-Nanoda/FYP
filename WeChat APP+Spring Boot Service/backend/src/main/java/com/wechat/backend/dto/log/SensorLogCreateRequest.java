package com.wechat.backend.dto.log;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;

public record SensorLogCreateRequest(
        @NotBlank(message = "topic is required")
        @Size(max = 64, message = "topic is too long")
        String topic,

        @Size(max = 32, message = "temperature is too long")
        String temperature,

        @Size(max = 32, message = "humidity is too long")
        String humidity,

        @Size(max = 32, message = "air is too long")
        String air,

        @Size(max = 32, message = "brightness is too long")
        String brightness,

        @Size(max = 64, message = "bemfaTime is too long")
        String bemfaTime,

        @Size(max = 2048, message = "rawPayload is too long")
        String rawPayload
) {
}
