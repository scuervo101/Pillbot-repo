; Auto-generated. Do not edit!


(cl:in-package fiducial_msgs-msg)


;//! \htmlinclude IdArray.msg.html

(cl:defclass <IdArray> (roslisp-msg-protocol:ros-message)
  ((ids
    :reader ids
    :initarg :ids
    :type (cl:vector cl:integer)
   :initform (cl:make-array 0 :element-type 'cl:integer :initial-element 0)))
)

(cl:defclass IdArray (<IdArray>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <IdArray>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'IdArray)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name fiducial_msgs-msg:<IdArray> is deprecated: use fiducial_msgs-msg:IdArray instead.")))

(cl:ensure-generic-function 'ids-val :lambda-list '(m))
(cl:defmethod ids-val ((m <IdArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader fiducial_msgs-msg:ids-val is deprecated.  Use fiducial_msgs-msg:ids instead.")
  (ids m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <IdArray>) ostream)
  "Serializes a message object of type '<IdArray>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'ids))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    ))
   (cl:slot-value msg 'ids))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <IdArray>) istream)
  "Deserializes a message object of type '<IdArray>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'ids) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'ids)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296)))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<IdArray>)))
  "Returns string type for a message object of type '<IdArray>"
  "fiducial_msgs/IdArray")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'IdArray)))
  "Returns string type for a message object of type 'IdArray"
  "fiducial_msgs/IdArray")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<IdArray>)))
  "Returns md5sum for a message object of type '<IdArray>"
  "4f22efebf407aadba2ecc69df353d113")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'IdArray)))
  "Returns md5sum for a message object of type 'IdArray"
  "4f22efebf407aadba2ecc69df353d113")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<IdArray>)))
  "Returns full string definition for message of type '<IdArray>"
  (cl:format cl:nil "#Id Array to store found fiducials~%~%int32[] ids~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'IdArray)))
  "Returns full string definition for message of type 'IdArray"
  (cl:format cl:nil "#Id Array to store found fiducials~%~%int32[] ids~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <IdArray>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'ids) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <IdArray>))
  "Converts a ROS message object to a list"
  (cl:list 'IdArray
    (cl:cons ':ids (ids msg))
))
