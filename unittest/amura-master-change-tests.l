;;; -*- mode: lisp -*-

;; for issue #422
(deftest fix-issue422 ()
 (values
  (ash #xffffffff -31)
  (ash #xffffffff -32)
  (ash #xfffffffff -32)
  (ash -1 -32)
  (ash (- #xfffffffff) -32))
 => 1
 => 0
 => 15
 => -1
 => -16)

;; for issue #426
(deftest fix-issue426 ()
 (values
  (last '(1 2 3 4))
  (last '(1 2 . 3)))
 => (4)
 => (2 . 3))
(deftest fix-issue426-circular ()
 (let ((l '(1 2 3)))
   (setf (cdr (last l)) l)
   (last l))
 !! simple-program-error)

;; ;;; amura-master-change-tests.l ends here.
