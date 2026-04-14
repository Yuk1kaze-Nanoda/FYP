package com.wechat.backend.repository;

import com.wechat.backend.entity.SensorLog;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.time.LocalDateTime;

public interface SensorLogRepository extends JpaRepository<SensorLog, Long> {

    @Query("""
            select s from SensorLog s
            join s.user u
            where (:phone is null or u.phone = :phone)
              and (:startTime is null or s.createdAt >= :startTime)
              and (:endTime is null or s.createdAt <= :endTime)
            """)
    Page<SensorLog> search(
            @Param("phone") String phone,
            @Param("startTime") LocalDateTime startTime,
            @Param("endTime") LocalDateTime endTime,
            Pageable pageable
    );
}
