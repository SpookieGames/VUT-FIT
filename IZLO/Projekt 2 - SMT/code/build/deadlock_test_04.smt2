(set-logic UFLIA)

(set-info :status sat)

(declare-fun is_process (Int) Bool)
(declare-fun is_resource (Int) Bool)

(define-fun owns ((p Int) (r Int)) Bool
  (or
    (and (= p 1) (= r 42))
    (and (= p 2) (= r 999))
  )
)

(define-fun requests ((p Int) (r Int)) Bool
  (or
    (and (= p 1) (= r 999))
    (and (= p 2) (= r 42))
  )
)

(define-fun in_deadlock ((p Int)) Bool
  (or
    (= p 1)
    (= p 2)
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

(assert
  (and
    (is_process 1)
    (is_process 2)
    (is_resource 42)
    (is_resource 999)

    has_deadlock
  )
)

(check-sat)
