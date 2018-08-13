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
	  *lua-indent-start-at-function*

	  lua-mode
	  lua-indent-line
	  lua-indent-line-and-next-line
	  lua-newline-and-indent
	  lua-toggle-comment-out
	  lua-electric-insert
	  lua-electric-close
	  lua-completion
	  lua-autoindent-d
	  lua-autoindent-e))

(defvar *lua-mode-hook*             nil)
(defvar *lua-mode-map*              nil)
(defvar *lua-mode-abbrev-table*     nil)
(defvar *lua-mode-syntax-table*     nil)
(defvar *lua-completion-list*       nil)

(defvar *lua-indent-level*            2)
(defvar *lua-label-offset*            2)
(defvar *lua-tab-always-indent*     nil)
(defvar *lua-indent-tabs-mode*        t)
(defvar *lua-indent-start-at-function* nil)

(defvar *lua-comment-column*         32)

(defvar *lua-keyword-file*        "Lua")
(defvar *lua-keyword-hash-table*    nil)

(set-extended-key-translate-table exkey-C-tab #\C-F24)

(unless *lua-mode-map*
  (setq *lua-mode-map* (make-sparse-keymap))
  (define-key *lua-mode-map* #\{      'lua-electric-insert)
  (define-key *lua-mode-map* #\}      'lua-electric-close)
  (define-key *lua-mode-map* #\d      'lua-autoindent-d)
  (define-key *lua-mode-map* #\e      'lua-autoindent-e)
  (define-key *lua-mode-map* #\TAB    'lua-indent-line)
  (define-key *lua-mode-map* #\M-TAB  'lua-completion)
  (define-key *lua-mode-map* #\C-F24  'lua-indent-line-and-next-line)
  (define-key *lua-mode-map* #\RET    'lua-newline-and-indent)
  (define-key *lua-mode-map* #\C-\'   'lua-toggle-comment-out)
  (define-key *lua-mode-map* #\C-h    'backward-delete-char-untabify-or-selection))

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

(defun lua-debug-current-position ()
  (save-excursion
    (let ((debug-format
	   (lambda (&rest args)
	     (save-excursion
	       (with-set-buffer
		 (set-buffer (get-buffer-create "*Debug Out*"))
		 (end-of-buffer)
		 (insert (apply 'format (cons nil args)))))))
	  (get-current-line-string
	   (lambda ()
	     (save-excursion
	       (buffer-substring (progn (goto-bol) (point))
				 (progn (goto-eol) (point)))))))
      (funcall debug-format "~4D:~A~%"
	       (current-line-number) (funcall get-current-line-string))
      (let ((columnstr "     "))
	(dotimes (x (current-column))
	  (setq columnstr (concat columnstr " ")))
	(setq columnstr (format nil "~A^(~D)" columnstr (current-column)))
	(cond ((eq (parse-point-syntax) :string)
	       (setq columnstr (concat columnstr " STRING ")))
	      ((eq (parse-point-syntax) :comment)
	       (setq columnstr (concat columnstr " COMMENT")))
	      (t
	       (setq columnstr (concat columnstr " NORMAL "))))
	(when (bolp)
	  (setq columnstr (concat columnstr " bolp")))
	(when (eolp)
	  (setq columnstr (concat columnstr " eolp")))
	(when (equal (point) (point-min))
	  (setq columnstr (concat columnstr " pmin")))
	(when (equal (point) (point-max))
	  (setq columnstr (concat columnstr " pmax")))
	(funcall debug-format "~A~%" columnstr)
	))))

(defun lua-point-column (p)
  (save-excursion
    (goto-char p)
    (current-column)))

(defun lua-skip-indent-chars ()
  (skip-chars-forward " \t"))

(defun lua-skip-indent-from-bol ()
  (goto-bol)
  (lua-skip-indent-chars))

(defun lua-previous-not-blank-line (&optional ignore_indent_chars)
  (loop
    (unless (previous-line)
      (return nil))
    (goto-bol)
    (when ignore_indent_chars
      (lua-skip-indent-chars))
    (unless (eolp)
      (return t))))

(defun lua-previous-not-blank-not-comment-out-line ()
  (loop
    (unless (lua-previous-not-blank-line t)
      (return nil))
    (when (and (eq (parse-point-syntax) nil)
	       (not (looking-at *lua-singleline-comment-regexp*))
	       (bolp))
      (return t))))

(defun lua-calc-indent-offset ()
  (save-excursion
    (lua-skip-indent-from-bol)
    (let ((column 0))
      (cond ((looking-at *lua-label-regexp*)
	     (incf column *lua-label-offset*))
	    ((looking-at *lua-block-mid-regexp*)
	     (decf column *lua-indent-level*)))
      column)))

(defun lua-multiline-comment-terminated-column ()
  (save-excursion
    (goto-bol)
    (unless (eq (parse-point-syntax) :comment)
      (return nil))
    (while (and (eq (parse-point-syntax) :comment)
		(not (eolp)))
      (forward-char))
    (if (eq (parse-point-syntax) :comment)
	nil
      (current-column))))

(defun lua-count-current-indentation ()
  (save-excursion
    (lua-skip-indent-from-bol)
    (current-column)))

(defun lua-count-previous-comment-indentation ()
  (save-excursion
    (loop
      (unless (lua-previous-not-blank-line t)
	(return 0))
      (goto-eol)
      (while (and (eq (parse-point-syntax) :comment)
		  (not (bolp)))
	(backward-char))

      (if (and (eq (parse-point-syntax) :comment)
	       (bolp))
	  (return (lua-count-current-indentation))
	(progn
	  (looking-at "--\\[=*\\[\\([-=\\[]+\\)?[ \t]*\\(.+\\)?")
	  (cond ((match-string 1)
		 (return (lua-point-column (match-beginning 0))))
		((match-string 2)
		 (return (lua-point-column (match-beginning 2))))
		(t
		 (return (lua-point-column (match-beginning 0))))))))))

(defun lua-calc-indent-in-comment-mid (&optional when-electric-indenting)
  (if when-electric-indenting
      (lua-count-current-indentation)
    (lua-count-previous-comment-indentation)))

(defun find-multiline-comment-start (bracket-equal)
  (save-excursion
    (let ((long-comment-start
	   (compile-regexp (concat "--\\(\\[" bracket-equal "\\[\\)"))))
      (loop
	(unless (scan-buffer long-comment-start
			     :no-dup t :limit (point-min) :reverse t)
	  (return-from find-multiline-comment-start nil))
	(unless (eq (parse-point-syntax) :comment)
	  (return))))
    (list (match-beginning 0) (match-beginning 1))))

(defun lua-calc-indent-in-comment-end (comment-end-column)
  (goto-bol)
  (let ((bol (point)))
    (goto-column comment-end-column)
    (unless  (scan-buffer *lua-multiline-comment-end-regexp*
			  :no-dup t :reverse t :limit bol)
      (return 0)))
  
  (let ((end-bracket-start-column (lua-point-column (match-beginning 0)))
	(end-bracket-equal        (match-string    1))
	(current-indentation      (lua-count-current-indentation)))
    (goto-column end-bracket-start-column)
    (skip-chars-backward " \t")
    (when (bolp)
      (return (lua-point-column
	       (cadr (find-multiline-comment-start end-bracket-equal)))))

    (goto-column end-bracket-start-column)
    (skip-chars-backward "-=]")
    (skip-chars-backward " \t")
    (when (bolp)
      (return (lua-point-column
	       (car (find-multiline-comment-start end-bracket-equal)))))

    (return (lua-count-previous-comment-indentation))))

(defun lua-calc-indent-in-comment (&optional when-electric-indenting)
  (save-excursion
    (let ((comment-end-column (lua-multiline-comment-terminated-column)))
      (if comment-end-column
	  (lua-calc-indent-in-comment-end comment-end-column)
	(lua-calc-indent-in-comment-mid when-electric-indenting)))))

(defun lua-calc-indent-normal ()
  (save-excursion
    (let ((target-point  nil))
      (goto-bol)
      (setq target-point (point))
      (lua-skip-indent-chars)
      (when (looking-for *lua-comment-out-string*)
	(return 0))
      (loop
	(unless (lua-previous-not-blank-not-comment-out-line)
	  (return-from lua-calc-indent-normal 0))
	(save-excursion
	  (let ((columns nil))
	    (lua-skip-indent-from-bol)
	    (let ((baseline-indent
		   (- (current-column) (lua-calc-indent-offset))))
	      (push (list nil baseline-indent)
		    columns))
	    (goto-bol)

	    (loop
	      (if (scan-buffer *lua-block-word-regexp* :limit target-point)
		  (let ((keyword       (or (match-string    3) (match-string    5)))
			(keyword-start (or (match-beginning 3) (match-beginning 5)))
			(keyword-end   (or (match-end       3) (match-end       5)))
			(keyword-column nil))
		    (goto-char keyword-start)
		    (setq keyword-column (current-column))
		    
		    (when (eq (parse-point-syntax) nil)
		      (cond
		       ((string= keyword "function")
			(goto-char keyword-end)
			(skip-chars-forward " \t\r\n")
			(if (and *lua-indent-start-at-function* (looking-for "("))
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

		;else
		(progn
		  (goto-char target-point)
		  (lua-skip-indent-from-bol)
		  (if (looking-at *lua-block-end-regexp*)
		      (if (caar columns)
			  (return-from lua-calc-indent-normal (max 0 (caar columns)))
			(return))
		    (if (looking-at *lua-block-mid-regexp*)
			(return-from lua-calc-indent-normal (+ (cadar columns) (lua-calc-indent-offset)))
		      (return-from lua-calc-indent-normal (cadar columns))))))
	      )
	    ))))))


(defun lua-calc-indent (&optional when-electric-indenting)
  (save-excursion
    (goto-bol)
    (cond
     ((and when-electric-indenting
	   (looking-at "[ \t]*$"))
      0)
     ((eq (parse-point-syntax) :comment)
      (lua-calc-indent-in-comment when-electric-indenting))
     (t
      (lua-calc-indent-normal)))))

(defun lua-indent-line (&optional when-electric-indenting)
  (interactive "*")
  (cond
   ((or (interactive-p)
	when-electric-indenting
	*lua-tab-always-indent*
	(save-excursion (skip-chars-backward " \t") (bolp)))
    (let ((original-column (current-column))
	  (original-indent (save-excursion
			     (lua-skip-indent-from-bol)
			     (current-column))))
      (unless (eq (save-excursion (goto-bol) (parse-point-syntax)) :string)
	(smart-indentation (lua-calc-indent when-electric-indenting))
	(goto-column (+ (save-excursion
			  (lua-skip-indent-from-bol)
			  (current-column))
			(- original-column original-indent)))
	(when (save-excursion
		(skip-chars-backward " \t")
		(bolp))
	  (lua-skip-indent-chars)))))
   (t
    (insert "\t"))))

(defun lua-indent-line-and-next-line ()
  (interactive "*")
  (lua-indent-line)
  (next-line))

(defun lua-newline-and-indent (&optional (arg 1))
  (interactive "*p")
  (unless (eq (parse-point-syntax) :string)
    (delete-trailing-spaces))
  (let ((eol (point)))
    (insert #\LFD arg)
    (save-excursion
      (goto-char eol)
      (lua-indent-line t)))
  (lua-indent-line))

(defun lua-comment-indent ()
  (save-excursion
    (let ((opoint (point)))
      (lua-skip-indent-from-bol)
      (cond
       ((looking-for *lua-comment-out-string*)
	0)
       ((or (looking-for comment)
	    (eolp))
	(lua-calc-indent))
       ((zerop comment-column)
	0)
       (t
	(goto-char opoint)
	(skip-chars-backward " \t")
	(max (1+ (current-column)) comment-column))))))

(defun lua-toggle-comment-out ()
  (interactive "*")
  (save-excursion
    (lua-skip-indent-from-bol)
    (if (looking-for *lua-comment-out-string*)
	(progn
	  (delete-char (length *lua-comment-out-string*))
	  (lua-indent-line))
      (progn
	(goto-bol)
	(insert *lua-comment-out-string* " "))))
  (next-line)
  (lua-skip-indent-from-bol))

(defun lua-electric-insert (&optional (arg 1))
  (interactive "*p")
  (self-insert-command arg)
  (lua-indent-line t))

(defun lua-electric-close (&optional (arg 1))
  (interactive "*p")
  (self-insert-command arg)
  (lua-indent-line t)
  (save-excursion
    (forward-char -1)
    (and (goto-matched-parenthesis)
	 (show-matched-parenthesis)))
  t)

; 直前にstrがあればインデントする関数fnを作る関数
(defun mk-lua-autoindent (fn str)
  (setf (symbol-function fn)
	(lambda (&optional (arg 1))
	  (interactive "*p")
	  (self-insert-command arg)
	  (if (and (not (eq (parse-point-syntax) :string)) (looking-back str))
	      (lua-indent-line t)))))

; endを打ち終わったらインデント
(mk-lua-autoindent 'lua-autoindent-d "end")

; elseを打ち終わったらインデント
(mk-lua-autoindent 'lua-autoindent-e "else")

(defun lua-completion ()
  "キーワードから補完"
  (interactive)
  (or *lua-completion-list*
      (setq *lua-completion-list* (make-list-from-keyword-table *lua-keyword-hash-table*))
      (return-from lua-completion nil))
  (let ((opoint (point)))
    (when (skip-syntax-spec-backward "w_")
      (let ((from (point)))
	(goto-char opoint)
	(do-completion from opoint :list *lua-completion-list*)))))

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

  (run-hooks '*lua-mode-hook*))
