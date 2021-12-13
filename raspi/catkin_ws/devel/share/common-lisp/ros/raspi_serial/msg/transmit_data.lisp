; Auto-generated. Do not edit!


(cl:in-package raspi_serial-msg)


;//! \htmlinclude transmit_data.msg.html

(cl:defclass <transmit_data> (roslisp-msg-protocol:ros-message)
  ((tx_string
    :reader tx_string
    :initarg :tx_string
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass transmit_data (<transmit_data>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <transmit_data>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'transmit_data)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name raspi_serial-msg:<transmit_data> is deprecated: use raspi_serial-msg:transmit_data instead.")))

(cl:ensure-generic-function 'tx_string-val :lambda-list '(m))
(cl:defmethod tx_string-val ((m <transmit_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader raspi_serial-msg:tx_string-val is deprecated.  Use raspi_serial-msg:tx_string instead.")
  (tx_string m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <transmit_data>) ostream)
  "Serializes a message object of type '<transmit_data>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'tx_string))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'tx_string))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <transmit_data>) istream)
  "Deserializes a message object of type '<transmit_data>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'tx_string) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'tx_string)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<transmit_data>)))
  "Returns string type for a message object of type '<transmit_data>"
  "raspi_serial/transmit_data")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'transmit_data)))
  "Returns string type for a message object of type 'transmit_data"
  "raspi_serial/transmit_data")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<transmit_data>)))
  "Returns md5sum for a message object of type '<transmit_data>"
  "6991089b0d19ddc329f4db889c0f15a2")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'transmit_data)))
  "Returns md5sum for a message object of type 'transmit_data"
  "6991089b0d19ddc329f4db889c0f15a2")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<transmit_data>)))
  "Returns full string definition for message of type '<transmit_data>"
  (cl:format cl:nil "# Message buffer for transmitting data~%~%uint8[] tx_string~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'transmit_data)))
  "Returns full string definition for message of type 'transmit_data"
  (cl:format cl:nil "# Message buffer for transmitting data~%~%uint8[] tx_string~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <transmit_data>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'tx_string) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <transmit_data>))
  "Converts a ROS message object to a list"
  (cl:list 'transmit_data
    (cl:cons ':tx_string (tx_string msg))
))
