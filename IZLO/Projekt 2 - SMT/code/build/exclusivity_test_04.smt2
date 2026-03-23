(set-logic UFLIA)
(set-info :status unsat)

(declare-fun requests (Int Int) Bool)
(declare-fun in_deadlock (Int) Bool)

(define-fun is_process ((p Int)) Bool
  (= (mod p 2) 0)
)

(define-fun is_resource ((r Int)) Bool
  (= (mod r 2) 1)
)

(define-fun owns ((p Int) (r Int)) Bool
  (or
    (= (+ p 1) r)
    (= (+ p -1) r)
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
      (owns p1 r) ; Proces p1 vlastni r
      (owns p2 r) ; Proces p2 vlastni r
    )
    (= p1 p2) ; Z toho vypliva ze p1 a p2 su rovnaky proces co vlastni nejaky zdroj r
  )
  )
)

;; Formule 2
(define-fun finitely_many_processes () Bool
  (exists ((p1 Int) (p2 Int))
  (forall ((p Int))
    (=>
      (is_process p)
      (and (<= p1 p) (<= p p2)) ; p1 <= p a p <= p2
    )
  )
  )
)

;; Formule 3
(define-fun has_deadlock () Bool
  (and
  ; Mnozina obsahuje aspon jeden proces
  (exists ((p Int))
    (and (in_deadlock p) (is_process p))
  )
  
  ; Kazdy proces caka na zdroj vlastneny procesom v mnozine
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
