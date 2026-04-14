package com.wechat.backend.dto.auth;

import java.time.LocalDateTime;

public record RegisterResponse(
        Long userId,
        String phone,
        LocalDateTime createdAt
) {
}
