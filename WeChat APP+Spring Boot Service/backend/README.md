# Backend Service (Spring Boot)

This backend provides:
- user register/login with JWT
- sensor log storage
- operation log storage
- public paginated log query APIs

## Requirements
- Java 17+
- Maven 3.9+
- MySQL 8+

## Run

```bash
cd backend
mvn spring-boot:run
```

Default server port: `8080`

## Config (In File)

All key runtime parameters are configured directly in:
- `src/main/resources/application.yml`

Current defaults:
- `server.port`: `8080`
- `url`: `jdbc:mysql://127.0.0.1:3306/smart_home?...`
- `username`: `root`
- `password`: `root`
- `app.jwt.secret`: `please-change-this-jwt-secret-to-32-plus-characters`
- `app.jwt.expiration-hours`: `24`

## API Overview

- `POST /api/auth/register`
- `POST /api/auth/login`
- `GET /api/auth/me` (JWT required)
- `POST /api/logs/sensors` (JWT required)
- `POST /api/logs/operations` (JWT required)
- `GET /api/logs/sensors` (public)
- `GET /api/logs/operations` (public)
