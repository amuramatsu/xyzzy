;;; -*- Mode: Lisp; Package: EDITOR -*-
;;;
;;; This file is part of xyzzy.
;;;

(provide "cmdbar")

(in-package "editor")

(export '(find-command-bar define-command-bar
	  show-command-bar hide-command-bar
	  delete-command-bar
	  init-command-bars load-command-bar-info
	  std-tool-bar buffer-bar))

(define-history-variable *command-bar-list* nil)
(defvar *command-bar-list-multi* (make-hash-table))
(defvar *defer-define-command-bar-list* (make-hash-table))
(defvar *command-bar-loaded* (make-hash-table))

(defun set-command-bar-list (frame val)
  (setq *command-bar-list* val)
  (si:*puthash frame *command-bar-list-multi* val))

(defun get-command-bar-list(frame)
  (let ((ret (gethash frame *command-bar-list-multi*)))
    (if ret
	ret
      (progn
	(set-command-bar-list frame (copy-tree *command-bar-list*))
	(gethash frame *command-bar-list-multi*)))))


(generate-hash-accessor command-bar-loaded)
(generate-hash-accessor defer-define-command-bar-list)


;;  0        1        2         3         4    5 6
;; (pkg-name sym-name disp-name visible-p edge x y)
(defmacro command-bar-package (bar) `(car ,bar))
(defmacro command-bar-symbol-name (bar) `(cadr ,bar))
(defmacro command-bar-display-name (bar) `(caddr ,bar))
(defmacro command-bar-visible-p (bar) `(nth 3 ,bar))
(defmacro command-bar-position (bar) `(cdr (nthcdr 3 ,bar)))
(defmacro command-bar-edge (bar) `(nth 4 ,bar))
(defmacro command-bar-xy (bar) `(cdr (nthcdr 4 ,bar)))

(defun find-command-bar (sym &optional(frame (selected-frame)))
  (let ((pkg (package-name (symbol-package sym)))
	(name (symbol-name sym)))
    (find-if #'(lambda (x)
		 (and (equal (car x) pkg)
		      (equal (cadr x) name)))
	     (get-command-bar-list frame))))

(defun command-bar-update (bar)
  (if (command-bar-visible-p bar) :check))

(defun define-command-bar (sym name &optional edge x y w (frame (selected-frame)))
  (cond ((null (get-command-bar-loaded frame))
	 (set-defer-define-command-bar-list frame
	    (nconc (delete sym (get-defer-define-command-bar-list frame)
			      :test #'eq :key #'car)
		      (list (list sym name edge x y w)))))
	((not (find-command-bar sym frame))
	 (let ((bar (list (package-name (symbol-package sym))
			  (symbol-name sym) name nil edge x y w)))
	   (set-command-bar-list frame (nconc (get-command-bar-list frame) (list bar)))
	   (add-menu-item (get-command-bar-popup-menu frame) bar name
			  #'(lambda () (interactive) (toggle-command-bar sym))
			  #'(lambda () (command-bar-update bar))))))
  t)

(defun show-command-bar-1 (frame bar sym)
  (unless (tool-bar-exist-p sym frame)
    (funcall sym))
  (apply #'show-tool-bar sym (append (command-bar-position bar) (list frame)))
  (setf (command-bar-visible-p bar) t)
  t)

(defun hide-command-bar-1 (frame bar sym)
  (let ((r (multiple-value-list (hide-tool-bar sym frame))))
    (when (car r)
      (setf (command-bar-position bar) r)
      (setf (command-bar-visible-p bar) nil)))
  t)


(defun toggle-command-bar-1 (frame bar sym)
  (if (command-bar-visible-p bar)
      (hide-command-bar-1 frame bar sym)
    (show-command-bar-1 frame bar sym)))

(defun delete-command-bar-1 (bar sym frame)
  (when (tool-bar-exist-p sym frame)
    (delete-tool-bar sym))
  (delete-menu (get-command-bar-popup-menu frame) bar nil)
  (set-command-bar-list frame (delete bar (get-command-bar-list frame)))
  t)

(defun show-command-bar (sym &optional (frame (selected-frame)))
  (let ((bar (find-command-bar sym frame)))
    (and bar (show-command-bar-1 frame bar sym))))

(defun hide-command-bar (sym &optional (frame (selected-frame)))
  (let ((bar (find-command-bar sym frame)))
    (and bar (hide-command-bar-1 frame bar sym))))

(defun toggle-command-bar (sym &optional (frame (selected-frame)))
  (let ((bar (find-command-bar sym frame)))
    (and bar (toggle-command-bar-1 frame bar sym))))

(defun named-command-bar-update (bar)
  (if (find-package (command-bar-package bar))
      (if (command-bar-visible-p bar) :check)
    :disable))

(defun toggle-named-command-bar (bar)
  (let ((pkg (find-package (command-bar-package bar))))
    (when pkg
      (toggle-command-bar-1 (selected-frame) bar (intern (command-bar-symbol-name bar) pkg)))))

(defun delete-command-bar (sym &optional (frame (selected-frame)))
  (let ((bar (find-command-bar sym frame)))
    (and bar (delete-command-bar-1 bar sym frame))))

(defun save-command-bar-info ()
  (mapc #'(lambda (edge list)
	    (mapc #'(lambda (info)
		      (let ((bar (find-command-bar (car info) (selected-frame))))
			(when bar
			  (setf (command-bar-edge bar) edge)
			  (setf (command-bar-xy bar) (cdr info)))))
		  list))
	'(:left :top :right :bottom) (list-tool-bars)))

(defun load-command-bar-info (&optional (frame (selected-frame)))
  (set-command-bar-loaded frame t)
  (or (get-command-bar-popup-menu frame)
      (set-command-bar-popup-menu frame (create-popup-menu)))
  (mapc #'(lambda (bar)
	    (ignore-errors
	     (let* ((pkg (find-package (command-bar-package bar)))
		    (sym (and pkg (intern (command-bar-symbol-name bar) pkg)))
		    (disp-name (command-bar-display-name bar)))
	       (cond (sym
		      (let ((x (find sym (get-defer-define-command-bar-list frame) :key #'car)))
			(when x
			  (setq disp-name (cadr x))))
		      (add-menu-item (get-command-bar-popup-menu frame) bar disp-name
				     #'(lambda ()
					 (interactive)
					 (toggle-command-bar sym))
				     #'(lambda () (command-bar-update bar)))
		      (when (command-bar-visible-p bar)
			(setf (command-bar-visible-p bar) nil)
			(show-command-bar-1 frame bar sym)))
		     (t
		      (add-menu-item (get-command-bar-popup-menu frame) bar disp-name
				     #'(lambda ()
					 (interactive)
					 (toggle-named-command-bar bar))
				     #'(lambda () (named-command-bar-update bar))))))))
	(get-command-bar-list frame))
  (mapc #'(lambda (x) (apply #'define-command-bar x))
	(get-defer-define-command-bar-list frame)))

(add-hook '*save-history-hook* #'save-command-bar-info)
(add-hook '*load-history-hook* #'load-command-bar-info)

(defun init-command-bars(frame)
  (define-command-bar 'std-tool-bar "�W��(&S)" nil nil nil nil frame)
  (define-command-bar 'buffer-bar "�o�b�t�@(&B)" nil nil nil nil frame))

(init-command-bars (selected-frame))

(setf (symbol-function 'buffer-bar) #'create-buffer-bar)

(defun tool-bar-update-search ()
  (or *last-search-p* :disable))

(defun std-tool-bar ()
  (create-tool-bar
   'std-tool-bar
   (merge-pathnames "toolbar.bmp" (etc-path))
   '(("�V�K�쐬" 24 new-file)
     ("�J��" 22 open-file-dialog)
     ("�㏑���ۑ�" 20 save-buffer-dialog :modified)
     ("�S�ĕۑ�" 75 save-all-buffers)
     :sep
     ("���" 29 print-selected-buffer-dialog)
     :sep
     ("�؂���" 7 kill-selection-to-clipboard :modify-any-selection)
     ("�R�s�[" 9 copy-selection-to-clipboard :any-selection)
     ("�\��t��" 2 paste-from-clipboard :clipboard)
     :sep
     ("���ɖ߂�" 0 undo :undo)
     ("����ς茳�ɖ߂��Ȃ�" 1 redo :redo)
     :sep
     ("����" 5 search-dialog)
     ("�O������" 3 repeat-backward-search tool-bar-update-search)
     ("��������" 4 repeat-forward-search tool-bar-update-search)
     :sep
     ("grep" 74 grep-dialog)
     :sep
     ("�܂�Ԃ��Ȃ�" 76
      set-buffer-fold-type-none set-buffer-fold-type-none-update)
     ("�w��ʒu�Ő܂�Ԃ�" 77
      set-buffer-fold-type-column set-buffer-fold-type-column-update)
     ("�E�B���h�E���Ő܂�Ԃ�" 78
      set-buffer-fold-type-window set-buffer-fold-type-window-update)
    )))