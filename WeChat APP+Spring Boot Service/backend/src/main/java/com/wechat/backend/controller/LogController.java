package com.wechat.backend.controller;

import com.wechat.backend.dto.common.CreateLogResponse;
import com.wechat.backend.dto.common.PagedResponse;
import com.wechat.backend.dto.log.OperationLogCreateRequest;
import com.wechat.backend.dto.log.OperationLogResponse;
import com.wechat.backend.dto.log.SensorLogCreateRequest;
import com.wechat.backend.dto.log.SensorLogResponse;
import com.wechat.backend.security.AuthenticatedUser;
import com.wechat.backend.service.LogService;
import jakarta.validation.Valid;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;

@RestController
@RequestMapping("/api/logs")
public class LogController {

    private final LogService logService;

    public LogController(LogService logService) {
        this.logService = logService;
    }

    @PostMapping("/sensors")
    public ResponseEntity<CreateLogResponse> createSensorLog(
            @AuthenticationPrincipal AuthenticatedUser currentUser,
            @Valid @RequestBody SensorLogCreateRequest request
    ) {
        CreateLogResponse response = logService.createSensorLog(currentUser, request);
        return ResponseEntity.status(HttpStatus.CREATED).body(response);
    }

    @PostMapping("/operations")
    public ResponseEntity<CreateLogResponse> createOperationLog(
            @AuthenticationPrincipal AuthenticatedUser currentUser,
            @Valid @RequestBody OperationLogCreateRequest request
    ) {
        CreateLogResponse response = logService.createOperationLog(currentUser, request);
        return ResponseEntity.status(HttpStatus.CREATED).body(response);
    }

    @GetMapping("/sensors")
    public PagedResponse<SensorLogResponse> querySensorLogs(
            @RequestParam(defaultValue = "0") int page,
            @RequestParam(defaultValue = "20") int size,
            @RequestParam(required = false)
            @DateTimeFormat(iso = DateTimeFormat.ISO.DATE_TIME)
            LocalDateTime startTime,
            @RequestParam(required = false)
            @DateTimeFormat(iso = DateTimeFormat.ISO.DATE_TIME)
            LocalDateTime endTime,
            @RequestParam(required = false) String phone
    ) {
        return logService.querySensorLogs(page, size, startTime, endTime, phone);
    }

    @GetMapping("/operations")
    public PagedResponse<OperationLogResponse> queryOperationLogs(
            @RequestParam(defaultValue = "0") int page,
            @RequestParam(defaultValue = "20") int size,
            @RequestParam(required = false)
            @DateTimeFormat(iso = DateTimeFormat.ISO.DATE_TIME)
            LocalDateTime startTime,
            @RequestParam(required = false)
            @DateTimeFormat(iso = DateTimeFormat.ISO.DATE_TIME)
            LocalDateTime endTime,
            @RequestParam(required = false) String phone
    ) {
        return logService.queryOperationLogs(page, size, startTime, endTime, phone);
    }
}
