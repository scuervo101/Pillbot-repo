;; Auto-generated. Do not edit!


(when (boundp 'raspi_serial::transmit_data)
  (if (not (find-package "RASPI_SERIAL"))
    (make-package "RASPI_SERIAL"))
  (shadow 'transmit_data (find-package "RASPI_SERIAL")))
(unless (find-package "RASPI_SERIAL::TRANSMIT_DATA")
  (make-package "RASPI_SERIAL::TRANSMIT_DATA"))

(in-package "ROS")
;;//! \htmlinclude transmit_data.msg.html


(defclass raspi_serial::transmit_data
  :super ros::object
  :slots (_tx_string ))

(defmethod raspi_serial::transmit_data
  (:init
   (&key
    ((:tx_string __tx_string) (make-array 0 :initial-element 0 :element-type :char))
    )
   (send-super :init)
   (setq _tx_string __tx_string)
   self)
  (:tx_string
   (&optional __tx_string)
   (if __tx_string (setq _tx_string __tx_string)) _tx_string)
  (:serialization-length
   ()
   (+
    ;; uint8[] _tx_string
    (* 1    (length _tx_string)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8[] _tx_string
     (write-long (length _tx_string) s)
     (princ _tx_string s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8[] _tx_string
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _tx_string (make-array n :element-type :char))
     (replace _tx_string buf :start2 ptr-) (incf ptr- n))
   ;;
   self)
  )

(setf (get raspi_serial::transmit_data :md5sum-) "6991089b0d19ddc329f4db889c0f15a2")
(setf (get raspi_serial::transmit_data :datatype-) "raspi_serial/transmit_data")
(setf (get raspi_serial::transmit_data :definition-)
      "# Message buffer for transmitting data

uint8[] tx_string
")



(provide :raspi_serial/transmit_data "6991089b0d19ddc329f4db889c0f15a2")


