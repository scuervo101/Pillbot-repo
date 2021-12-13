;; Auto-generated. Do not edit!


(when (boundp 'raspi_serial::received_data)
  (if (not (find-package "RASPI_SERIAL"))
    (make-package "RASPI_SERIAL"))
  (shadow 'received_data (find-package "RASPI_SERIAL")))
(unless (find-package "RASPI_SERIAL::RECEIVED_DATA")
  (make-package "RASPI_SERIAL::RECEIVED_DATA"))

(in-package "ROS")
;;//! \htmlinclude received_data.msg.html


(defclass raspi_serial::received_data
  :super ros::object
  :slots (_rx_string ))

(defmethod raspi_serial::received_data
  (:init
   (&key
    ((:rx_string __rx_string) (make-array 0 :initial-element 0 :element-type :char))
    )
   (send-super :init)
   (setq _rx_string __rx_string)
   self)
  (:rx_string
   (&optional __rx_string)
   (if __rx_string (setq _rx_string __rx_string)) _rx_string)
  (:serialization-length
   ()
   (+
    ;; uint8[] _rx_string
    (* 1    (length _rx_string)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8[] _rx_string
     (write-long (length _rx_string) s)
     (princ _rx_string s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8[] _rx_string
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _rx_string (make-array n :element-type :char))
     (replace _rx_string buf :start2 ptr-) (incf ptr- n))
   ;;
   self)
  )

(setf (get raspi_serial::received_data :md5sum-) "0a67d14656b1137d1a229687aac529a2")
(setf (get raspi_serial::received_data :datatype-) "raspi_serial/received_data")
(setf (get raspi_serial::received_data :definition-)
      "# Message to store received_data

uint8[] rx_string
")



(provide :raspi_serial/received_data "0a67d14656b1137d1a229687aac529a2")


