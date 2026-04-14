package com.wechat.backend.dto.common;

import java.time.LocalDateTime;

public record CreateLogResponse(
        Long id,
        LocalDateTime createdAt
) {
}
