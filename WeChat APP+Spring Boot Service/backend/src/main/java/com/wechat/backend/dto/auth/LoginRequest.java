package com.wechat.backend.dto.auth;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;

public record LoginRequest(
        @NotBlank(message = "phone is required")
        @Pattern(regexp = "^1\\d{10}$", message = "phone must be 11 digits and start with 1")
        String phone,

        @NotBlank(message = "password is required")
        @Size(min = 6, max = 64, message = "password length must be between 6 and 64")
        String password
) {
}
