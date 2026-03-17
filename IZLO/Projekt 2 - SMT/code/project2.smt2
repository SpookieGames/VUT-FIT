;; Lineární aritmetika + neinterpretované funkce
(set-logic UFLIA)

(set-option :produce-models true)


;; ====================
;; DEKLARACE PREDIKATU
;; ====================

;; Predikát "identifikátor je proces"
(declare-fun is_process (Int) Bool)

;; Predikát "identifikátor je zdroj"
(declare-fun is_resource (Int) Bool)

;; Predikát "proces vlastní zdroj"
(declare-fun owns (Int Int) Bool)

;; Predikát "proces je pozastaven a čeká na zdroj"
(declare-fun requests (Int Int) Bool)

;; Predikát popisující množinu procesů, pro které je
;; ověřováno zda nastal deadlock
(declare-fun in_deadlock (Int) Bool)

(define-fun example () Bool
  ; Pro každý identifikátor ...
  (forall ((i Int))
    ; ... platí, že buď ...
    (or
      ; ... není proces ...
      (not (is_process i))
      ; ... nebo není zdroj.
      (not (is_resource i))
    )
  )
)

;; ==================
;; START OF SOLUTION
;; ==================

; XLOGIN: xholesm00

;; Formule 1
(define-fun exclusive_resources () Bool
  ; Zde nahraďte vaším řešením
  false
)

;; Formule 2
(define-fun finitely_many_processes () Bool
  ; Zde nahraďte vaším řešením
  false
)

;; Formule 3
(define-fun has_deadlock () Bool
  ; Zde nahraďte vaším řešením
  false
)

;; ==================
;; END OF SOLUTION
;; ==================

; Základní testy ověřující, že podmínky jsou splnitelné

(set-info :status sat)
(check-sat-assuming (exclusive_resources))

(set-info :status sat)
(check-sat-assuming (finitely_many_processes))

(set-info :status sat)
(check-sat-assuming (has_deadlock))
