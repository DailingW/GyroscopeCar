//
//  ViewController.swift
//  BLE App
//
//  Created by Dailing Wu on 11/25/22.
//  Based on https://www.kodeco.com/231-core-bluetooth-tutorial-for-ios-heart-rate-monitor#toc-anchor-012
//

import UIKit
import CoreBluetooth
import CoreMotion
let ESP32ServiceCBUUID = CBUUID(string: "4fafc201-1fb5-459e-8fcc-c5c9c331914b")
let GyroXCharacteristicCBUUID = CBUUID(string: "beb5483e-36e1-4688-b7f5-ea07361b26a8")
let GyroYCharacteristicCBUUID = CBUUID(string: "bd2dbb6d-cf4c-4082-a827-a8b89edae723")
let GyroZCharacteristicCBUUID = CBUUID(string: "34304421-acb3-4ff4-ae78-884032e9d92a")

class ViewController: UIViewController {
    
    @IBOutlet weak var statuslabel: UILabel!
    @IBOutlet weak var gyrox: UILabel!
    @IBOutlet weak var gyroy: UILabel!
    @IBOutlet weak var gyroz: UILabel!
    var centralManager: CBCentralManager!
    var ESP32Peripheral: CBPeripheral!
    var motion = CMMotionManager()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        centralManager = CBCentralManager(delegate: self, queue: nil)
        statuslabel.text = "nothing going on"
        self.view.backgroundColor = UIColor.red
        self.gyrox.text = "---"
        self.gyroy.text = "---"
        self.gyroz.text = "---"
        //myGyro()
    }
    /*
    func myGyro(){
        motion.deviceMotionUpdateInterval = 0.5
        motion.startDeviceMotionUpdates(to: OperationQueue.current!){ (data, error) in
            if let trueData = data{
                self.gyrox.text = "\(trueData.attitude.pitch)"
                self.gyroy.text = "\(trueData.attitude.roll)"
                self.gyroz.text = "\(trueData.attitude.yaw)"
            }
        }
    }
     */
    override var shouldAutorotate: Bool{
        return false
    }
    override var supportedInterfaceOrientations: UIInterfaceOrientationMask{
        return .landscapeRight
    }
}
extension ViewController: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .unknown:
            print("central.state is .unknown")
        case .resetting:
            print("central.state is .resetting")
        case .unsupported:
            print("central.state is .unsupported")
        case .unauthorized:
            print("central.state is .unauthorized")
        case .poweredOff:
            print("central.state is .poweredOff")
        case .poweredOn:
            print("central.state is .poweredOn")
            self.statuslabel.text = "scanning for ESP32"
            self.view.backgroundColor = UIColor.yellow
            //centralManager.scanForPeripherals(withServices: nil)
            centralManager.scanForPeripherals(withServices: [ESP32ServiceCBUUID])
        }
    }
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                        advertisementData: [String: Any], rssi RSSI: NSNumber) {
        print(peripheral)
        ESP32Peripheral = peripheral
        ESP32Peripheral.delegate = self
        centralManager.stopScan()
        centralManager.connect(ESP32Peripheral)
        
    }
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("Connected!")
        ESP32Peripheral.discoverServices(nil)
        self.statuslabel.text = "ESP32 connected"
        self.view.backgroundColor = UIColor.green
    }
}
extension ViewController: CBPeripheralDelegate {
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services else { return }

        for service in services {
            print(service)
            peripheral.discoverCharacteristics(nil, for: service)
        }
    }
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService,
                    error: Error?) {
        guard let characteristics = service.characteristics else { return }
        motion.deviceMotionUpdateInterval = 0.5
        motion.startDeviceMotionUpdates(to: OperationQueue.current!){ (data, error) in
            if let trueData = data{
                for characteristic in characteristics {
                    print(characteristic)
                    if(characteristic.uuid == GyroXCharacteristicCBUUID){
                        self.gyrox.text = "\(trueData.attitude.pitch)"
                        let d = Data("\(trueData.attitude.pitch)".utf8)
                        peripheral.writeValue(d, for: characteristic, type: CBCharacteristicWriteType.withResponse)
                        //peripheral.readValue(for: characteristic)
                    }else if(characteristic.uuid == GyroYCharacteristicCBUUID){
                        self.gyroy.text = "\(trueData.attitude.roll)"
                        let d = Data("\(trueData.attitude.roll)".utf8)
                        peripheral.writeValue(d, for: characteristic, type: CBCharacteristicWriteType.withResponse)
                        //peripheral.readValue(for: characteristic)
                    }else if(characteristic.uuid == GyroZCharacteristicCBUUID){
                        self.gyroz.text = "\(trueData.attitude.yaw)"
                        let d = Data("\(trueData.attitude.yaw)".utf8)
                        peripheral.writeValue(d, for: characteristic, type: CBCharacteristicWriteType.withResponse)
                        //peripheral.readValue(for: characteristic)
                    }
                    //let d = Data("hello esp32".utf8)
                    //peripheral.writeValue(d, for: characteristic, type: CBCharacteristicWriteType.withResponse)
                    //peripheral.readValue(for: characteristic)
                }
            }
        }
        
    }
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic,
                    error: Error?) {
      switch characteristic.uuid {
        case GyroXCharacteristicCBUUID:
          print(String(data: Data(bytes: characteristic.value!), encoding: .utf8) ?? "no value")
        case GyroYCharacteristicCBUUID:
          print(String(data: Data(bytes: characteristic.value!), encoding: .utf8) ?? "no value")
        case GyroZCharacteristicCBUUID:
          print(String(data: Data(bytes: characteristic.value!), encoding: .utf8) ?? "no value")
        default:
          print("Unhandled Characteristic UUID: \(characteristic.uuid)")
      }
    }
}
