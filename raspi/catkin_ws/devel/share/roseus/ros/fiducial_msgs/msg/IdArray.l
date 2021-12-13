;; Auto-generated. Do not edit!


(when (boundp 'fiducial_msgs::IdArray)
  (if (not (find-package "FIDUCIAL_MSGS"))
    (make-package "FIDUCIAL_MSGS"))
  (shadow 'IdArray (find-package "FIDUCIAL_MSGS")))
(unless (find-package "FIDUCIAL_MSGS::IDARRAY")
  (make-package "FIDUCIAL_MSGS::IDARRAY"))

(in-package "ROS")
;;//! \htmlinclude IdArray.msg.html


(defclass fiducial_msgs::IdArray
  :super ros::object
  :slots (_ids ))

(defmethod fiducial_msgs::IdArray
  (:init
   (&key
    ((:ids __ids) (make-array 0 :initial-element 0 :element-type :integer))
    )
   (send-super :init)
   (setq _ids __ids)
   self)
  (:ids
   (&optional __ids)
   (if __ids (setq _ids __ids)) _ids)
  (:serialization-length
   ()
   (+
    ;; int32[] _ids
    (* 4    (length _ids)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32[] _ids
     (write-long (length _ids) s)
     (dotimes (i (length _ids))
       (write-long (elt _ids i) s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32[] _ids
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _ids (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _ids i) (sys::peek buf ptr- :integer)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get fiducial_msgs::IdArray :md5sum-) "4f22efebf407aadba2ecc69df353d113")
(setf (get fiducial_msgs::IdArray :datatype-) "fiducial_msgs/IdArray")
(setf (get fiducial_msgs::IdArray :definition-)
      "#Id Array to store found fiducials

int32[] ids
")



(provide :fiducial_msgs/IdArray "4f22efebf407aadba2ecc69df353d113")

