(set-logic UFLIA)
(set-info :status sat)

(declare-fun in_deadlock (Int) Bool)

(define-fun is_process ((p Int)) Bool true)
(define-fun is_resource ((r Int)) Bool false)
(define-fun owns ((p Int) (r Int)) Bool true)
(define-fun requests ((p Int) (r Int)) Bool true)

;; START OF SOLUTION
;; ==================

; XLOGIN: xholesm00

;; Formule 1
(define-fun exclusive_resources () Bool
  (forall ((r Int) (p1 Int) (p2 Int))
  (=>
    (and
      (is_resource r)
      (is_process p1)
      (is_process p2)
      (owns p1 r)
      (owns p2 r)
    )
    (= p1 p2)
  )
)
)

;; Formule 2
(define-fun finitely_many_processes () Bool
  (exists ((min Int) (max Int))
  (forall ((p Int))
    (=>
      (is_process p)
      (and (<= min p) (<= p max))
    )
  )
)
)

;; Formule 3
(define-fun has_deadlock () Bool
  (and
  ; Condition 1: The set contains at least one process
  (exists ((p Int))
    (and (in_deadlock p) (is_process p))
  )
  
  ; Condition 2: Every process in the set waits for a resource owned by another process in the set
  (forall ((p Int))
    (=>
      (and (in_deadlock p) (is_process p))
      (exists ((r Int) (owner Int))
        (and
          (is_resource r)
          (is_process owner)
          (requests p r)
          (owns owner r)
          (in_deadlock owner)
        )
      )
    )
  )
)
)

;; ==================
;; END OF SOLUTION

(assert exclusive_resources)

(check-sat)
