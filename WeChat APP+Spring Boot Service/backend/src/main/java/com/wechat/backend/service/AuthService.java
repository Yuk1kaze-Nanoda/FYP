package com.wechat.backend.service;

import com.wechat.backend.dto.auth.LoginRequest;
import com.wechat.backend.dto.auth.LoginResponse;
import com.wechat.backend.dto.auth.RegisterRequest;
import com.wechat.backend.dto.auth.RegisterResponse;
import com.wechat.backend.dto.auth.UserProfileResponse;
import com.wechat.backend.entity.User;
import com.wechat.backend.repository.UserRepository;
import com.wechat.backend.security.AuthenticatedUser;
import com.wechat.backend.security.JwtService;
import org.springframework.http.HttpStatus;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.server.ResponseStatusException;

import java.time.Instant;

@Service
public class AuthService {

    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;
    private final JwtService jwtService;

    public AuthService(UserRepository userRepository, PasswordEncoder passwordEncoder, JwtService jwtService) {
        this.userRepository = userRepository;
        this.passwordEncoder = passwordEncoder;
        this.jwtService = jwtService;
    }

    @Transactional
    public RegisterResponse register(RegisterRequest request) {
        if (userRepository.existsByPhone(request.phone())) {
            throw new ResponseStatusException(HttpStatus.CONFLICT, "Phone already registered");
        }

        User user = new User();
        user.setPhone(request.phone());
        user.setPasswordHash(passwordEncoder.encode(request.password()));
        user.touchForCreate();

        User saved = userRepository.save(user);
        return new RegisterResponse(saved.getId(), saved.getPhone(), saved.getCreatedAt());
    }

    @Transactional(readOnly = true)
    public LoginResponse login(LoginRequest request) {
        User user = userRepository.findByPhone(request.phone())
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Account does not exist"));

        if (!passwordEncoder.matches(request.password(), user.getPasswordHash())) {
            throw new ResponseStatusException(HttpStatus.UNAUTHORIZED, "Password is incorrect");
        }

        String token = jwtService.generateToken(user.getId(), user.getPhone());
        Instant expiresAt = jwtService.parseToken(token).expiration();

        return new LoginResponse(token, expiresAt, new UserProfileResponse(user.getId(), user.getPhone()));
    }

    public UserProfileResponse me(AuthenticatedUser currentUser) {
        return new UserProfileResponse(currentUser.id(), currentUser.phone());
    }
}
