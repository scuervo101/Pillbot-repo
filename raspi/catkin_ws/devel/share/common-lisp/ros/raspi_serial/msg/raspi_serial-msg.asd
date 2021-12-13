
(cl:in-package :asdf)

(defsystem "raspi_serial-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "received_data" :depends-on ("_package_received_data"))
    (:file "_package_received_data" :depends-on ("_package"))
    (:file "transmit_data" :depends-on ("_package_transmit_data"))
    (:file "_package_transmit_data" :depends-on ("_package"))
  ))