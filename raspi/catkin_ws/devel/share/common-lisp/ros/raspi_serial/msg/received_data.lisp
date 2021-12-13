; Auto-generated. Do not edit!


(cl:in-package raspi_serial-msg)


;//! \htmlinclude received_data.msg.html

(cl:defclass <received_data> (roslisp-msg-protocol:ros-message)
  ((rx_string
    :reader rx_string
    :initarg :rx_string
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass received_data (<received_data>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <received_data>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'received_data)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name raspi_serial-msg:<received_data> is deprecated: use raspi_serial-msg:received_data instead.")))

(cl:ensure-generic-function 'rx_string-val :lambda-list '(m))
(cl:defmethod rx_string-val ((m <received_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader raspi_serial-msg:rx_string-val is deprecated.  Use raspi_serial-msg:rx_string instead.")
  (rx_string m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <received_data>) ostream)
  "Serializes a message object of type '<received_data>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'rx_string))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'rx_string))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <received_data>) istream)
  "Deserializes a message object of type '<received_data>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'rx_string) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'rx_string)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<received_data>)))
  "Returns string type for a message object of type '<received_data>"
  "raspi_serial/received_data")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'received_data)))
  "Returns string type for a message object of type 'received_data"
  "raspi_serial/received_data")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<received_data>)))
  "Returns md5sum for a message object of type '<received_data>"
  "0a67d14656b1137d1a229687aac529a2")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'received_data)))
  "Returns md5sum for a message object of type 'received_data"
  "0a67d14656b1137d1a229687aac529a2")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<received_data>)))
  "Returns full string definition for message of type '<received_data>"
  (cl:format cl:nil "# Message to store received_data~%~%uint8[] rx_string~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'received_data)))
  "Returns full string definition for message of type 'received_data"
  (cl:format cl:nil "# Message to store received_data~%~%uint8[] rx_string~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <received_data>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'rx_string) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <received_data>))
  "Converts a ROS message object to a list"
  (cl:list 'received_data
    (cl:cons ':rx_string (rx_string msg))
))
