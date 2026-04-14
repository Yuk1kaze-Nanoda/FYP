package com.wechat.backend.service;

import com.wechat.backend.dto.common.CreateLogResponse;
import com.wechat.backend.dto.common.PagedResponse;
import com.wechat.backend.dto.log.OperationLogCreateRequest;
import com.wechat.backend.dto.log.OperationLogResponse;
import com.wechat.backend.dto.log.SensorLogCreateRequest;
import com.wechat.backend.dto.log.SensorLogResponse;
import com.wechat.backend.entity.OperationLog;
import com.wechat.backend.entity.OperationResult;
import com.wechat.backend.entity.SensorLog;
import com.wechat.backend.entity.User;
import com.wechat.backend.repository.OperationLogRepository;
import com.wechat.backend.repository.SensorLogRepository;
import com.wechat.backend.repository.UserRepository;
import com.wechat.backend.security.AuthenticatedUser;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;

import java.time.LocalDateTime;
import java.util.List;

@Service
public class LogService {

    private final UserRepository userRepository;
    private final SensorLogRepository sensorLogRepository;
    private final OperationLogRepository operationLogRepository;

    public LogService(
            UserRepository userRepository,
            SensorLogRepository sensorLogRepository,
            OperationLogRepository operationLogRepository
    ) {
        this.userRepository = userRepository;
        this.sensorLogRepository = sensorLogRepository;
        this.operationLogRepository = operationLogRepository;
    }

    @Transactional
    public CreateLogResponse createSensorLog(AuthenticatedUser currentUser, SensorLogCreateRequest request) {
        User user = findCurrentUser(currentUser.id());

        SensorLog log = new SensorLog();
        log.setUser(user);
        log.setTopic(request.topic());
        log.setTemperature(request.temperature());
        log.setHumidity(request.humidity());
        log.setAir(request.air());
        log.setBrightness(request.brightness());
        log.setBemfaTime(request.bemfaTime());
        log.setRawPayload(request.rawPayload());
        log.touchForCreate();

        SensorLog saved = sensorLogRepository.save(log);
        return new CreateLogResponse(saved.getId(), saved.getCreatedAt());
    }

    @Transactional
    public CreateLogResponse createOperationLog(AuthenticatedUser currentUser, OperationLogCreateRequest request) {
        User user = findCurrentUser(currentUser.id());

        OperationLog log = new OperationLog();
        log.setUser(user);
        log.setTopic(request.topic());
        log.setCommandKey(request.commandKey());
        log.setCommandLabel(request.commandLabel());
        log.setResult(OperationResult.valueOf(request.result()));
        log.setBemfaResponse(request.bemfaResponse());
        log.touchForCreate();

        OperationLog saved = operationLogRepository.save(log);
        return new CreateLogResponse(saved.getId(), saved.getCreatedAt());
    }

    @Transactional(readOnly = true)
    public PagedResponse<SensorLogResponse> querySensorLogs(
            int page,
            int size,
            LocalDateTime startTime,
            LocalDateTime endTime,
            String phone
    ) {
        Pageable pageable = PageRequest.of(page, size, Sort.by(Sort.Direction.DESC, "createdAt"));
        Page<SensorLog> result = sensorLogRepository.search(normalizePhone(phone), startTime, endTime, pageable);

        List<SensorLogResponse> content = result.getContent().stream()
                .map(log -> new SensorLogResponse(
                        log.getId(),
                        log.getUser().getPhone(),
                        log.getTopic(),
                        log.getTemperature(),
                        log.getHumidity(),
                        log.getAir(),
                        log.getBrightness(),
                        log.getBemfaTime(),
                        log.getRawPayload(),
                        log.getCreatedAt()
                ))
                .toList();

        return new PagedResponse<>(content, result.getNumber(), result.getSize(), result.getTotalElements(), result.getTotalPages());
    }

    @Transactional(readOnly = true)
    public PagedResponse<OperationLogResponse> queryOperationLogs(
            int page,
            int size,
            LocalDateTime startTime,
            LocalDateTime endTime,
            String phone
    ) {
        Pageable pageable = PageRequest.of(page, size, Sort.by(Sort.Direction.DESC, "createdAt"));
        Page<OperationLog> result = operationLogRepository.search(normalizePhone(phone), startTime, endTime, pageable);

        List<OperationLogResponse> content = result.getContent().stream()
                .map(log -> new OperationLogResponse(
                        log.getId(),
                        log.getUser().getPhone(),
                        log.getTopic(),
                        log.getCommandKey(),
                        log.getCommandLabel(),
                        log.getResult().name(),
                        log.getBemfaResponse(),
                        log.getCreatedAt()
                ))
                .toList();

        return new PagedResponse<>(content, result.getNumber(), result.getSize(), result.getTotalElements(), result.getTotalPages());
    }

    private User findCurrentUser(Long userId) {
        return userRepository.findById(userId)
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.UNAUTHORIZED, "User not found"));
    }

    private String normalizePhone(String phone) {
        if (phone == null || phone.isBlank()) {
            return null;
        }
        return phone.trim();
    }
}
