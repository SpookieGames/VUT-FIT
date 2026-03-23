(set-logic UFLIA)

(set-info :status sat)

(define-fun is_process ((p Int)) Bool
  (and
    (>= p 0)
    (< p 21)
  )
)
(define-fun is_resource ((r Int)) Bool
  (>= r 100)
)

(define-fun owns ((p Int) (r Int)) Bool
  (and
    (is_process p)
    (= (+ p 100) r)
  )
)

(define-fun requests ((p Int) (r Int)) Bool
  (or
    (and (= p 20) (= r 100))
    (and
      (is_process p)
      (is_resource r)
      (= (+ p 101) r)
    )
  )
)

(define-fun in_deadlock ((p Int)) Bool
  (and
    (>= p 0)
    (< p 21)
  )
)

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

(assert has_deadlock)

(check-sat)
