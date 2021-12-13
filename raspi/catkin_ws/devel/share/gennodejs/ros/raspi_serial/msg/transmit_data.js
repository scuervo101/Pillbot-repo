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

class transmit_data {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.tx_string = null;
    }
    else {
      if (initObj.hasOwnProperty('tx_string')) {
        this.tx_string = initObj.tx_string
      }
      else {
        this.tx_string = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type transmit_data
    // Serialize message field [tx_string]
    bufferOffset = _arraySerializer.uint8(obj.tx_string, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type transmit_data
    let len;
    let data = new transmit_data(null);
    // Deserialize message field [tx_string]
    data.tx_string = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.tx_string.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'raspi_serial/transmit_data';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '6991089b0d19ddc329f4db889c0f15a2';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # Message buffer for transmitting data
    
    uint8[] tx_string
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new transmit_data(null);
    if (msg.tx_string !== undefined) {
      resolved.tx_string = msg.tx_string;
    }
    else {
      resolved.tx_string = []
    }

    return resolved;
    }
};

module.exports = transmit_data;
