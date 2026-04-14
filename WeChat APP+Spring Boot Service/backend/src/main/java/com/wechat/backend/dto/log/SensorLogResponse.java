package com.wechat.backend.dto.log;

import java.time.LocalDateTime;

public record SensorLogResponse(
        Long id,
        String phone,
        String topic,
        String temperature,
        String humidity,
        String air,
        String brightness,
        String bemfaTime,
        String rawPayload,
        LocalDateTime createdAt
) {
}
