(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "ni/update-xyzzy"))

(deftest safe-data-read ()
  "ni::safe-data-read �� read-time-eval �̃��[�_�[�}�N�������s���Ȃ����Ƃ��m�F�B
   ni::load-site-data-file �����l�̏����Ȃ̂ł�����̃e�X�g�͏ȗ��B"
  (with-temp-files (file)
    (with-open-file (out file :direction :output :if-does-not-exist :create)
      (format out "#.(+ 1 1)~%"))
    (ni::safe-data-read file))
  !! simple-error)