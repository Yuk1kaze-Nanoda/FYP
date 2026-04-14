package com.wechat.backend.dto.log;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;

public record OperationLogCreateRequest(
        @NotBlank(message = "topic is required")
        @Size(max = 64, message = "topic is too long")
        String topic,

        @NotBlank(message = "commandKey is required")
        @Size(max = 32, message = "commandKey is too long")
        String commandKey,

        @NotBlank(message = "commandLabel is required")
        @Size(max = 64, message = "commandLabel is too long")
        String commandLabel,

        @NotBlank(message = "result is required")
        @Pattern(regexp = "SUCCESS|FAIL", message = "result must be SUCCESS or FAIL")
        String result,

        @Size(max = 2048, message = "bemfaResponse is too long")
        String bemfaResponse
) {
}
