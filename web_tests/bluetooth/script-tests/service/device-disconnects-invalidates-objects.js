'use strict';
bluetooth_test(() => {
  return setBluetoothFakeAdapter('DisconnectingHealthThermometerAdapter')
    .then(() => requestDeviceWithTrustedClick({
      filters: [{services: ['health_thermometer']}],
      optionalServices: [request_disconnection_service_uuid]}))
    .then(device => device.gatt.connect())
    .then(gattServer => {
      let characteristics;
      return gattServer.getPrimaryService('health_thermometer')
        .then(service => service.CALLS([
          getCharacteristic('measurement_interval')|
          getCharacteristics()|
          getCharacteristics('measurement_interval')[UUID]]))
        .then(c => {
          // Convert to array if necessary.
          characteristics = [].concat(c);
          return get_request_disconnection(gattServer);
        })
        .then(requestDisconnection => requestDisconnection())
        .then(() => gattServer.connect())
        .then(() => characteristics);
    })
    .then(characteristics => {
      let promises = Promise.resolve();
      for (let characteristic of characteristics) {
        let error = new DOMException(
          'Characteristic with UUID ' + characteristic.uuid +
          ' is no longer valid. Remember to retrieve the ' +
          'characteristic again after reconnecting.',
          'InvalidStateError');
        promises = promises.then(() =>
          assert_promise_rejects_with_message(
            characteristic.readValue(),
            error));
        promises = promises.then(() =>
          assert_promise_rejects_with_message(
            characteristic.writeValue(new Uint8Array([1])),
            error));
        promises = promises.then(() =>
          assert_promise_rejects_with_message(
            characteristic.startNotifications(),
            error));
        promises = promises.then(() =>
          assert_promise_rejects_with_message(
            characteristic.stopNotifications(),
            error));
      }
      return promises;
    });
}, 'Calls on characteristics after device disconnects and we reconnect. ' +
   'Should reject with InvalidStateError.');