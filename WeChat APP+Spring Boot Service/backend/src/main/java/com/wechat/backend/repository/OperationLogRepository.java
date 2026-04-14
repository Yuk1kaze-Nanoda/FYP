package com.wechat.backend.repository;

import com.wechat.backend.entity.OperationLog;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.time.LocalDateTime;

public interface OperationLogRepository extends JpaRepository<OperationLog, Long> {

    @Query("""
            select o from OperationLog o
            join o.user u
            where (:phone is null or u.phone = :phone)
              and (:startTime is null or o.createdAt >= :startTime)
              and (:endTime is null or o.createdAt <= :endTime)
            """)
    Page<OperationLog> search(
            @Param("phone") String phone,
            @Param("startTime") LocalDateTime startTime,
            @Param("endTime") LocalDateTime endTime,
            Pageable pageable
    );
}
