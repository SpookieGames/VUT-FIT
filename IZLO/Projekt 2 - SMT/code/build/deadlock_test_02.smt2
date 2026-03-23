(set-logic UFLIA)
(set-info :status sat)

(declare-fun is_process (Int) Bool)
(declare-fun is_resource (Int) Bool)

(define-fun owns ((p Int) (r Int)) Bool
  (and
    (= p 157)
    (or
      (= r 2)
      (= r 3)
      (= r 4)
    )
  )
)


(define-fun requests ((p Int) (r Int)) Bool
  (and
    (= p 157)
    (= r 3)
  )
)

(define-fun in_deadlock ((p Int)) Bool
  (= p 157)
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
    (is_process 157)
    (is_resource 2)
    (is_resource 3)
    (is_resource 4)

    has_deadlock
  )
)

(check-sat)
