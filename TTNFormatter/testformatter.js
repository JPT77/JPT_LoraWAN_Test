
/**
 * TTN Payload Formatter (Uplink Decoder)
 * Für JPT_LoraWAN_Test Projekt
 * 
 * Payload-Struktur (6 Bytes):
 * Byte 0: TX-Reason
 * Byte 1: TX Power (dBm)
 * Byte 2: Datarate (DR0-DR5)
 * Byte 3: Spreading Factor (SF7-SF12)
 * Byte 4-5: Supply Voltage (mV, 16-bit Big Endian)
 */

function decodeUplink(input) {
  var data = {};
  var warnings = [];
  var errors = [];

  // Mindestens 6 Bytes erwartet
  if (input.bytes.length < 6) {
    errors.push("Payload zu kurz: " + input.bytes.length + " Bytes (erwartet: 6)");
    return {
      data: data,
      warnings: warnings,
      errors: errors
    };
  }

  // Byte 0: TX-Reason
  var txReasonCode = input.bytes[0];
  var txReasonMap = {
    0: "Timer Event",
    1: "User Button Event",
    2: "Input Event",
    3: "FUOTA Event",
    4: "App Cycle Event",
    5: "Timeout Event",
    255: "Undefined Event"
  };
  data.tx_reason_code = txReasonCode;
  data.tx_reason = txReasonMap[txReasonCode] || "Unbekannt (" + txReasonCode + ")";

  // Byte 1: TX Power (dBm) - als signed int8
  var txPower = input.bytes[1];
  if (txPower > 127) {
    txPower = txPower - 256; // Signed conversion
  }
  data.tx_power_dbm = txPower;

  // Byte 2: Datarate (DR0-DR5)
  var datarate = input.bytes[2];
  data.datarate = datarate;
  data.datarate_name = "DR" + datarate;

  // Byte 3: Spreading Factor (SF7-SF12)
  var spreadingFactor = input.bytes[3];
  data.spreading_factor = spreadingFactor;
  data.spreading_factor_name = "SF" + spreadingFactor;

  // Byte 4-5: Supply Voltage (mV, Big Endian)
  var supplyVoltage = (input.bytes[4] << 8) | input.bytes[5];
  data.supply_voltage_mv = supplyVoltage;
  data.supply_voltage_v = parseFloat((supplyVoltage / 1000).toFixed(3));

  // Batterie-Status Schätzung
  if (supplyVoltage >= 3000) {
    data.battery_status = "Gut";
  } else if (supplyVoltage >= 2700) {
    data.battery_status = "Mittel";
  } else if (supplyVoltage >= 2400) {
    data.battery_status = "Niedrig";
  } else {
    data.battery_status = "Kritisch";
    warnings.push("Batteriespannung kritisch niedrig!");
  }

  // Bandbreite aus Datarate ableiten (EU868)
  var bandwidthMap = {
    0: 125, // DR0: SF12/125kHz
    1: 125, // DR1: SF11/125kHz
    2: 125, // DR2: SF10/125kHz
    3: 125, // DR3: SF9/125kHz
    4: 125, // DR4: SF8/125kHz
    5: 125, // DR5: SF7/125kHz
    6: 250  // DR6: SF7/250kHz (falls verwendet)
  };
  data.bandwidth_khz = bandwidthMap[datarate] || 125;

  // Bitrate Schätzung (EU868)
  var bitrateMap = {
    0: 250,   // DR0: SF12/125kHz
    1: 440,   // DR1: SF11/125kHz
    2: 980,   // DR2: SF10/125kHz
    3: 1760,  // DR3: SF9/125kHz
    4: 3125,  // DR4: SF8/125kHz
    5: 5470   // DR5: SF7/125kHz
  };
  data.bitrate_bps = bitrateMap[datarate] || 0;

  return {
    data: data,
    warnings: warnings,
    errors: errors
  };
}
