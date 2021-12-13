;; Auto-generated. Do not edit!


(when (boundp 'central_master_controller::command)
  (if (not (find-package "CENTRAL_MASTER_CONTROLLER"))
    (make-package "CENTRAL_MASTER_CONTROLLER"))
  (shadow 'command (find-package "CENTRAL_MASTER_CONTROLLER")))
(unless (find-package "CENTRAL_MASTER_CONTROLLER::COMMAND")
  (make-package "CENTRAL_MASTER_CONTROLLER::COMMAND"))

(in-package "ROS")
;;//! \htmlinclude command.msg.html


(defclass central_master_controller::command
  :super ros::object
  :slots (_cmd ))

(defmethod central_master_controller::command
  (:init
   (&key
    ((:cmd __cmd) 0)
    )
   (send-super :init)
   (setq _cmd (round __cmd))
   self)
  (:cmd
   (&optional __cmd)
   (if __cmd (setq _cmd __cmd)) _cmd)
  (:serialization-length
   ()
   (+
    ;; int32 _cmd
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _cmd
       (write-long _cmd s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _cmd
     (setq _cmd (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get central_master_controller::command :md5sum-) "66990e73c7aab0c47ddcdc70f7fa5bd0")
(setf (get central_master_controller::command :datatype-) "central_master_controller/command")
(setf (get central_master_controller::command :definition-)
      "# Command message which the cm_controller subscribe too

int32 cmd
")



(provide :central_master_controller/command "66990e73c7aab0c47ddcdc70f7fa5bd0")


