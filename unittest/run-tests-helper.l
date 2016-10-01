(let ((dir (if *load-pathname*
               (directory-namestring *load-pathname*)
             ".")))
  (load-file (merge-pathnames "simple-test.l" dir))

  (defun process-command-line (arg)
    (cond ((string= "--run-all-tests" arg)
           (let ((*error-on-overwriting-test* t))
             (dolist (test (sort (directory dir :absolute t :recursive t :wild "*-tests.l")
                                   #'string-lessp))
                 (load-test-file test))))
          (t
           (load-test-file
              (merge-pathnames arg *command-line-args-base-directory*)))))
  (defun run-tests-and-exit ()
     (run-all-tests)
     (write-file "run-tests.log")
     (kill-xyzzy t))
  (add-hook '*process-command-line-hook* 'process-command-line)
  (when (eq si:*command-line-args* nil)
    (setq si:*command-line-args* '("--run-all-tests")))
  (add-hook '*post-startup-hook* 'run-tests-and-exit))
