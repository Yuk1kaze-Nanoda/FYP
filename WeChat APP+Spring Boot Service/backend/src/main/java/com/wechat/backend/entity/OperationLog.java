package com.wechat.backend.entity;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.FetchType;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Index;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Table;

import java.time.LocalDateTime;

@Entity
@Table(
        name = "operation_logs",
        indexes = {
                @Index(name = "idx_operation_logs_user_created_at", columnList = "user_id,created_at")
        }
)
public class OperationLog {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @ManyToOne(fetch = FetchType.LAZY, optional = false)
    @JoinColumn(name = "user_id", nullable = false)
    private User user;

    @Column(nullable = false, length = 64)
    private String topic;

    @Column(name = "command_key", nullable = false, length = 32)
    private String commandKey;

    @Column(name = "command_label", nullable = false, length = 64)
    private String commandLabel;

    @Enumerated(EnumType.STRING)
    @Column(nullable = false, length = 16)
    private OperationResult result;

    @Column(name = "bemfa_response", length = 2048)
    private String bemfaResponse;

    @Column(name = "created_at", nullable = false, updatable = false)
    private LocalDateTime createdAt;

    public Long getId() {
        return id;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public String getTopic() {
        return topic;
    }

    public void setTopic(String topic) {
        this.topic = topic;
    }

    public String getCommandKey() {
        return commandKey;
    }

    public void setCommandKey(String commandKey) {
        this.commandKey = commandKey;
    }

    public String getCommandLabel() {
        return commandLabel;
    }

    public void setCommandLabel(String commandLabel) {
        this.commandLabel = commandLabel;
    }

    public OperationResult getResult() {
        return result;
    }

    public void setResult(OperationResult result) {
        this.result = result;
    }

    public String getBemfaResponse() {
        return bemfaResponse;
    }

    public void setBemfaResponse(String bemfaResponse) {
        this.bemfaResponse = bemfaResponse;
    }

    public LocalDateTime getCreatedAt() {
        return createdAt;
    }

    public void touchForCreate() {
        this.createdAt = LocalDateTime.now();
    }
}
