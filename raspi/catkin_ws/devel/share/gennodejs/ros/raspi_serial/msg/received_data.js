// Auto-generated. Do not edit!

// (in-package raspi_serial.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class received_data {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.rx_string = null;
    }
    else {
      if (initObj.hasOwnProperty('rx_string')) {
        this.rx_string = initObj.rx_string
      }
      else {
        this.rx_string = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type received_data
    // Serialize message field [rx_string]
    bufferOffset = _arraySerializer.uint8(obj.rx_string, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type received_data
    let len;
    let data = new received_data(null);
    // Deserialize message field [rx_string]
    data.rx_string = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.rx_string.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'raspi_serial/received_data';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '0a67d14656b1137d1a229687aac529a2';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # Message to store received_data
    
    uint8[] rx_string
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new received_data(null);
    if (msg.rx_string !== undefined) {
      resolved.rx_string = msg.rx_string;
    }
    else {
      resolved.rx_string = []
    }

    return resolved;
    }
};

module.exports = received_data;
