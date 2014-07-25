;;; -*- Mode: Lisp; Package: EDITOR -*-
;;;
;;; This file is a part of lua support patch.
;;;

(provide "lua-mode")
(in-package "editor")
(export '(
	  *lua-mode-hook*
	  *lua-mode-map*
	  *lua-mode-abbrev-table*
	  *lua-mode-syntax-table*

	  *lua-indent-level*

	  *lua-label-offset*

	  *lua-tab-always-indent*

	  *lua-indent-tabs-mode*

	  *lua-comment-column*

	  *lua-use-encoding*
	  *lua-use-eol-code*

	  lua-mode
	  lua-indent-line
	  lua-indent-line-and-next-line
	  lua-newline-and-indent
	  lua-toggle-comment-out))

(defvar *lua-mode-hook*             nil)
(defvar *lua-mode-map*              nil)
(defvar *lua-mode-abbrev-table*     nil)
(defvar *lua-mode-syntax-table*     nil)

(defvar *lua-indent-level*            4)
(defvar *lua-label-offset*            4)
(defvar *lua-tab-always-indent*     nil)
(defvar *lua-indent-tabs-mode*        t)

(defvar *lua-comment-column*         32)

(defvar *lua-use-encoding*          nil)
(defvar *lua-use-eol-code*          nil)

(defvar *lua-keyword-file*        "Lua")
(defvar *lua-keyword-hash-table*    nil)

(set-extended-key-translate-table exkey-C-tab #\C-F24)

(unless *lua-mode-map*
  (setq *lua-mode-map* (make-sparse-keymap))
  (define-key *lua-mode-map* #\TAB    'lua-indent-line)
  (define-key *lua-mode-map* #\C-F24  'lua-indent-line-and-next-line)
  (define-key *lua-mode-map* #\RET    'lua-newline-and-indent)
  (define-key *lua-mode-map* #\C-\'   'lua-toggle-comment-out)
  (define-key *lua-mode-map* #\C-h    'delete-backward-char-or-selection))

(unless *lua-mode-syntax-table*
  (setq *lua-mode-syntax-table* (make-syntax-table))

  (set-syntax-option *lua-mode-syntax-table* *syntax-option-lua-long-bracket*)

  (set-syntax-whitespace *lua-mode-syntax-table* #\SPC)
  (set-syntax-whitespace *lua-mode-syntax-table* #\TAB)
  (set-syntax-whitespace *lua-mode-syntax-table* #\LFD)
  (set-syntax-whitespace *lua-mode-syntax-table* #\RET)

  (set-syntax-string *lua-mode-syntax-table* #\")
  (set-syntax-string *lua-mode-syntax-table* #\')

  (set-syntax-escape *lua-mode-syntax-table* #\\)

  (set-syntax-symbol *lua-mode-syntax-table* #\_)

  (set-syntax-match  *lua-mode-syntax-table* #\( #\))
  (set-syntax-match  *lua-mode-syntax-table* #\{ #\})
  (set-syntax-match  *lua-mode-syntax-table* #\[ #\])

  (set-syntax-start-c++-comment *lua-mode-syntax-table* #\-)
  (set-syntax-end-c++-comment   *lua-mode-syntax-table* #\LFD)

  ;; DO NOT set multi-comment when *syntax-option-lua-long-bracket* is enabled.
  ;;   set-syntax-start-multi-comment
  ;;   set-syntax-end-multi-comment
  )

(unless *lua-mode-abbrev-table*
  (define-abbrev-table '*lua-mode-abbrev-table*))

(defvar *lua-comment-start-string* "--")

(defvar *lua-comment-out-string* "--~")

(defvar *lua-block-word-regexp*
  (compile-regexp "\\(\\([^A-Za-z0-9_]\\|^\\)\\(function\\|while\\|for\\|do\\|if\\|then\\|repeat\\|elseif\\|end\\|until\\)\\([^A-Za-z0-9_]\\|$\\)\\)\\|\\((\\|)\\|{\\|}\\)"))

(defvar *lua-block-end-regexp*
  (compile-regexp "\\([)}]\\)\\|\\(\\(elseif\\|end\\|until\\)\\([^A-Za-z0-9_]\\|$\\)\\)"))

(defvar *lua-block-mid-regexp*
  (compile-regexp "\\(else\\)\\([^A-Za-z0-9_]\\|$\\)"))

(defvar *lua-label-regexp*
  (compile-regexp "::[a-zA-Z_]+[a-zA-Z0-9_]*::"))

(defvar *lua-multiline-comment-start-regexp*
  (compile-regexp "--\\[\\(=*\\)\\["))

(defvar *lua-multiline-comment-end-regexp*
  (compile-regexp "\\]\\(=*\\)\\]"))

(defvar *lua-singleline-comment-regexp*
  (compile-regexp "--\\($\\|\\([^\\[].*$\\)\\|\\(\\[[^=\\[].*$\\)\\|\\(\\[=+[^=\\[].*$\\)\\)"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun debug-format (&rest args)
  (save-excursion
    (with-set-buffer
      (set-buffer (get-buffer-create "*Debug Out*"))
      (end-of-buffer)
      (insert (apply 'format (cons nil args))))))

(defun get-current-line-string ()
  (save-excursion
    (buffer-substring (progn (goto-bol) (point))
		      (progn (goto-eol) (point)))))

(defun debug-current-position ()
  (save-excursion
    (debug-format "~4D:~A~%" (current-line-number) (get-current-line-string))
    (let ((columnstr "     "))
      (dotimes (x (current-column))
	(setq columnstr (concat columnstr " ")))
      (setq columnstr (concat columnstr (format nil "^(~D)" (current-column))))
      (cond ((equal (parse-point-syntax) :string)
	     (setq columnstr (concat columnstr " STRING ")))
	      ((equal (parse-point-syntax) :comment)
	       (setq columnstr (concat columnstr " COMMENT")))
	      (t
	       (setq columnstr (concat columnstr " NORMAL "))))
	(if (bolp)
	    (setq columnstr (concat columnstr " bolp")))
	(if (eolp)
	    (setq columnstr (concat columnstr " eolp")))
	(if (equal (point) (point-min))
	    (setq columnstr (concat columnstr " pmin")))
	(if (equal (point) (point-max))
	    (setq columnstr (concat columnstr " pmax")))
	(debug-format "~A~%" columnstr)
      )))

(defun point-column (p)
  (save-excursion
    (goto-char p)
    (current-column)))

(defun skip-indent-chars ()
  (skip-chars-forward " \t"))

(defun skip-indent-from-bol ()
  (goto-bol)
  (skip-indent-chars))

(defun previous-not-blank-line (&optional ignore_indent_chars)
  (loop
    (if (not (previous-line))
	(return nil))
    (goto-bol)
    (when ignore_indent_chars
      (skip-indent-chars))
    (if (not (eolp))
	(return t))))

(defun previous-not-blank-not-comment-out-line ()
  (loop
    (if (not (previous-not-blank-line t))
	(return nil))
    (if (and (equal (parse-point-syntax) nil)
	     (not   (looking-at *lua-singleline-comment-regexp*))
	     (bolp))
	(return t))))

(defun calc-lua-indent-offset ()
  (save-excursion
    (skip-indent-from-bol)
    (let ((column 0))
      (cond ((looking-at *lua-label-regexp*)
	     (incf column *lua-label-offset*))
	    ((looking-at *lua-block-mid-regexp*)
	     (decf column *lua-indent-level*)))
      column)))

(defun multiline-comment-terminated-column ()
  (save-excursion
    (goto-bol)

    (when (not (equal (parse-point-syntax) :comment))
      (return-from multiline-comment-terminated-column nil))

    (while (and (equal (parse-point-syntax) :comment)
		(not (eolp)))
      (forward-char))

    (if (not (equal (parse-point-syntax) :comment))
	(current-column)
      nil)))

(defun count-current-indentation ()
  (save-excursion
    (skip-indent-from-bol)
    (current-column)))

(defun count-previous-comment-indentation ()
  (save-excursion
    (loop
      (if (not (previous-not-blank-line t))
	  (return 0))

      (goto-eol)
      (while (and (equal (parse-point-syntax) :comment)
		  (not (bolp)))
	(backward-char))

      (if (and (equal (parse-point-syntax) :comment)
	       (bolp))
	  (return (count-current-indentation))
	(progn
	  (looking-at "--\\[=*\\[\\([-=\\[]+\\)?[ \t]*\\(.+\\)?")
	  (cond ((match-string 1)
		 (return (point-column (match-beginning 0))))
		((match-string 2)
		 (return (point-column (match-beginning 2))))
		(t
		 (return (point-column (match-beginning 0))))))))))

(defun calc-lua-indent-in-comment-mid (&optional when-inserting-new-line)

  (if when-inserting-new-line
      (count-current-indentation)
    (count-previous-comment-indentation)))

(defun find-multiline-comment-start (bracket-equal)
  (save-excursion
    (let ((long-comment-start (compile-regexp (concat "--\\(\\[" bracket-equal "\\[\\)"))))
      (loop
	(if (not (scan-buffer long-comment-start :no-dup t :limit (point-min) :reverse t))
	    (return-from find-multiline-comment-start nil))
	(if (not (equal (parse-point-syntax) :comment))
	    (return))))
    (list (match-beginning 0) (match-beginning 1))))

(defun calc-lua-indent-in-comment-end (comment-end-column)

  (let ((bol (progn (goto-bol) (point))))
    (goto-column comment-end-column)
    (if (not (scan-buffer *lua-multiline-comment-end-regexp* :no-dup t :reverse t :limit bol))
	(return-from calc-lua-indent-in-comment-end 0)))

  (let ((end-bracket-start-column (point-column (match-beginning 0)))
	(end-bracket-equal        (match-string    1))
	(current-indentation      (count-current-indentation)))

    (goto-column end-bracket-start-column)
    (skip-chars-backward " \t")
    (if (bolp)
	(return-from calc-lua-indent-in-comment-end
	  (point-column (cadr (find-multiline-comment-start end-bracket-equal)))))

    (goto-column end-bracket-start-column)
    (skip-chars-backward "-=]")
    (skip-chars-backward " \t")
    (if (bolp)
	(return-from calc-lua-indent-in-comment-end
	  (point-column (car (find-multiline-comment-start end-bracket-equal)))))

    (return-from calc-lua-indent-in-comment-end
      (count-previous-comment-indentation))))

(defun calc-lua-indent-in-comment (&optional when-inserting-new-line)
  (save-excursion
    (let ((comment-end-column  (multiline-comment-terminated-column)))
      (if (not comment-end-column)
	  (calc-lua-indent-in-comment-mid when-inserting-new-line)
	(calc-lua-indent-in-comment-end comment-end-column)))))

(defun calc-lua-indent-normal ()
  (save-excursion
    (let ((target-point  nil))

      (goto-bol)
      (setq target-point (point))

      (skip-indent-chars)
      (if (looking-for *lua-comment-out-string*)
	  (return-from calc-lua-indent-normal 0))

      (loop
	(if (not (previous-not-blank-not-comment-out-line))
	    (return-from calc-lua-indent-normal 0))

	(save-excursion
	  (let ((columns nil))
	    (skip-indent-from-bol)
	    (let ((baseline-indent (- (current-column) (calc-lua-indent-offset))))
	      (push (list nil baseline-indent)
		    columns))
	    (goto-bol)

	    (loop
	      (if (scan-buffer *lua-block-word-regexp* :limit target-point)
		  (let ((keyword       (or (match-string    3) (match-string    5)))
			(keyword-start (or (match-beginning 3) (match-beginning 5)))
			(keyword-end   (or (match-end       3) (match-end       5)))
			(keyword-column nil))

		    (setq keyword-column (progn
					   (goto-char keyword-start)
					   (current-column)))

		    (goto-char keyword-start)
		    (when (equal (parse-point-syntax) nil)
		      (cond
		       ((string= keyword "function")
			(goto-char keyword-end)
			(skip-chars-forward " \t\r\n")
			(if (looking-for "(")
			    (push (list keyword-column
					(+ keyword-column *lua-indent-level*)
					keyword)
				  columns)
			  (push (list (cadar columns)
				      (+ (cadar columns) *lua-indent-level*)
				      keyword)
				columns)))

		       ((or (string= keyword "do"))
			(if (> (list-length columns) 1)
			    (let ((k (caddar columns)))
			      (if (or (string= k "while")
				      (string= k "for"))
				  (pop columns))))
			(goto-char keyword-end)
			(skip-chars-forward " \t")
			(if (or (eolp)
				(looking-at *lua-singleline-comment-regexp*))
			    (push (list (cadar columns)
					(+ (cadar columns) *lua-indent-level*)
					keyword)
				  columns)
			  (push (list keyword-column
				      (current-column)
				      keyword)
				columns)))
		       ((or (string= keyword "then")
			    (string= keyword "elseif"))
			(if (> (list-length columns) 1)
			    (pop columns)
			  (return))
			(goto-char keyword-end)
			(skip-chars-forward " \t")
			(if (or (eolp)
				(looking-at *lua-singleline-comment-regexp*))
			    (push (list (cadar columns)
					(+ (cadar columns) *lua-indent-level*)
					keyword)
				  columns)
			  (push (list keyword-column
				      (current-column)
				      keyword)
				columns)))

		       ((or (string= keyword "while")
			    (string= keyword "for")
			    (string= keyword "repeat")
			    (string= keyword "if")
			    (string= keyword "(")
			    (string= keyword "{"))
			(goto-char keyword-end)
			(skip-chars-forward " \t")
			(if (or (eolp)
				(looking-at *lua-singleline-comment-regexp*))
			    (push (list (cadar columns)
					(+ (cadar columns) *lua-indent-level*)
					keyword)
				  columns)
			  (push (list keyword-column
				      (current-column)
				      keyword)
				columns)))

		       ((or (string= keyword "end")
			    (string= keyword "until")
			    (string= keyword ")")
			    (string= keyword "}"))
			(if (> (list-length columns) 1)
			    (pop columns)
			  (return)))))

		    (goto-char keyword-end))

		(progn
		  (goto-char target-point)
		  (skip-indent-from-bol)
		  (if (looking-at *lua-block-end-regexp*)
		      (if (not (caar columns))
			  (return)
			(return-from calc-lua-indent-normal
			  (max 0 (caar columns))))
		    (if (looking-at *lua-block-mid-regexp*)
			(return-from calc-lua-indent-normal
			  (+ (cadar columns) (calc-lua-indent-offset)))
		      (return-from calc-lua-indent-normal
			(cadar columns))))))
	      )
	    ))))))


(defun calc-lua-indent (when-inserting-new-line)
  (save-excursion
    (goto-bol)

    (if (and when-inserting-new-line
	     (looking-at "[ \t]*$"))
	0
      (if (equal (parse-point-syntax) :comment)
	  (calc-lua-indent-in-comment when-inserting-new-line)
	(calc-lua-indent-normal)))))

(defun lua-indent-line (&optional when-inserting-new-line)
  (interactive "*")

  (if (or (interactive-p)
	  *lua-tab-always-indent*
	  (save-excursion
	    (skip-chars-backward " \t")
	    (bolp)))

      (let ((original-column (current-column))
	    (original-indent (save-excursion
			       (skip-indent-from-bol)
			       (current-column))))

	(if (not (equal (save-excursion
			  (goto-bol)
			  (parse-point-syntax))
			:string))
	    (progn
	      (smart-indentation (calc-lua-indent when-inserting-new-line))

	      (goto-column (+ (save-excursion
				(skip-indent-from-bol)
				(current-column))
			      (- original-column original-indent)))

	      (if (save-excursion
		    (skip-chars-backward " \t")
		    (bolp))
		  (skip-indent-chars)))))

    (insert "\t")))

(defun lua-indent-line-and-next-line ()
  (interactive "*")

  (lua-indent-line)
  (next-line))

(defun lua-newline-and-indent (&optional (arg 1))
  (interactive "*p")

  (if (not (equal (parse-point-syntax) :string))
      (delete-trailing-spaces))

  (let ((eol (point)))
    (insert #\LFD arg)
    (save-excursion
      (goto-char eol)
      (lua-indent-line t)
      ))

  (lua-indent-line))

(defun lua-comment-indent ()
  (save-excursion
    (let ((opoint (point)))
      (skip-indent-from-bol)
      (cond
       ((looking-for *lua-comment-out-string*)
	0)
       (t
	(cond ((or (looking-for comment)
		     (eolp))
	       (funcall calc-lua-indent))
	      ((zerop comment-column)
	       0)
	      (t
	       (goto-char opoint)
	       (skip-chars-backward " \t")
	       (max (1+ (current-column)) comment-column))))))))

(defun lua-toggle-comment-out ()
  (interactive "*")

  (save-excursion
    (skip-indent-from-bol)
    (if (looking-for *lua-comment-out-string*)
	(progn
	  (delete-char (length *lua-comment-out-string*))
	  (lua-indent-line))
      (progn
	(goto-bol)
	(insert *lua-comment-out-string* " "))))

  (next-line)
  (skip-indent-from-bol))

(defun lua-mode ()
  (interactive)
  (kill-all-local-variables)
  (setq buffer-mode 'lua-mode)
  (setq mode-name "Lua")

  (use-keymap *lua-mode-map*)
  (use-syntax-table *lua-mode-syntax-table*)

  (make-local-variable 'indent-tabs-mode)
  (setq indent-tabs-mode *lua-indent-tabs-mode*)

  (and *lua-keyword-file*
       (null *lua-keyword-hash-table*)
       (setq *lua-keyword-hash-table*
	     (load-keyword-file *lua-keyword-file* t)))

  (when *lua-keyword-hash-table*
    (make-local-variable 'keyword-hash-table)
    (setq keyword-hash-table *lua-keyword-hash-table*))

  (setq *local-abbrev-table* *lua-mode-abbrev-table*)

  (setq comment-start "-- ")
  (setq comment-end "")
  (setq comment-start-skip "--[ \t]*")
  (setq comment-indent-function 'lua-comment-indent)
  (when *lua-comment-column*
    (setq comment-column *lua-comment-column*))

  (when *lua-use-encoding*
    (set-buffer-fileio-encoding *lua-use-encoding*))

  (when *lua-use-eol-code*
    (set-buffer-eol-code *lua-use-eol-code*))

  (run-hooks '*lua-mode-hook*))
