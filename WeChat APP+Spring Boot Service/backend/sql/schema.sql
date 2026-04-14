-- Smart Home backend schema (MySQL 8+)

CREATE DATABASE IF NOT EXISTS smart_home
  DEFAULT CHARACTER SET utf8mb4
  DEFAULT COLLATE utf8mb4_0900_ai_ci;

USE smart_home;

CREATE TABLE IF NOT EXISTS users (
  id BIGINT NOT NULL AUTO_INCREMENT,
  phone VARCHAR(20) NOT NULL,
  password_hash VARCHAR(100) NOT NULL,
  created_at DATETIME(6) NOT NULL,
  updated_at DATETIME(6) NOT NULL,
  PRIMARY KEY (id),
  CONSTRAINT uk_users_phone UNIQUE (phone),
  INDEX idx_users_phone (phone)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS sensor_logs (
  id BIGINT NOT NULL AUTO_INCREMENT,
  user_id BIGINT NOT NULL,
  topic VARCHAR(64) NOT NULL,
  temperature VARCHAR(32) NULL,
  humidity VARCHAR(32) NULL,
  air VARCHAR(32) NULL,
  brightness VARCHAR(32) NULL,
  bemfa_time VARCHAR(64) NULL,
  raw_payload VARCHAR(2048) NULL,
  created_at DATETIME(6) NOT NULL,
  PRIMARY KEY (id),
  INDEX idx_sensor_logs_user_created_at (user_id, created_at),
  CONSTRAINT fk_sensor_logs_user
    FOREIGN KEY (user_id) REFERENCES users(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS operation_logs (
  id BIGINT NOT NULL AUTO_INCREMENT,
  user_id BIGINT NOT NULL,
  topic VARCHAR(64) NOT NULL,
  command_key VARCHAR(32) NOT NULL,
  command_label VARCHAR(64) NOT NULL,
  result ENUM('SUCCESS', 'FAIL') NOT NULL,
  bemfa_response VARCHAR(2048) NULL,
  created_at DATETIME(6) NOT NULL,
  PRIMARY KEY (id),
  INDEX idx_operation_logs_user_created_at (user_id, created_at),
  CONSTRAINT fk_operation_logs_user
    FOREIGN KEY (user_id) REFERENCES users(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
