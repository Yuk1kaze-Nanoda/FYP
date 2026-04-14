package com.wechat.backend.dto.log;

import java.time.LocalDateTime;

public record OperationLogResponse(
        Long id,
        String phone,
        String topic,
        String commandKey,
        String commandLabel,
        String result,
        String bemfaResponse,
        LocalDateTime createdAt
) {
}
