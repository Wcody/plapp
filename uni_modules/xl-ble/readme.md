# xl-ble开发文档
## 购买说明
- 购买插件前请先试用，试用满意再购买。在试用中如遇到问题, 欢迎进入交流群, 将全力协助你使用本插件。

## 概述

本SDK提供了一套完整的蓝牙低功耗(BLE)设备交互接口，采用模块化设计，将功能接口与回调监听分离，便于开发者灵活使用。

- 支持蓝牙扫描、连接、断开;

- 支持字节数据、字符串数据写入;

- 蓝牙状态监听、连接状态监听、数据回传监听、数据写入是否完成监听;

- 支持GBK编码;


## 导入

```javascript
import * as ble from '@/uni_modules/xl-ble';
```

## 功能接口说明

####  扫描设备

```javascript
// 参数1: 发现设备回调
ble.startScan((device) => {
  console.log(`发现设备: ${JSON.stringify(device)}`);
},null);
```

#### 停止扫描

```javascript
ble.stopScan();
```



#### 连接设备

```javascript
// 参数1: 设备id
// 参数2(可选):指定服务与特征，未指定内部会自动匹配
ble.connect(
  deviceId,
  // {
  // 	seviceUuid: '0000ff00-0000-1000-8000-00805f9b34fb',
  // 	writeUuid: '0000ff02-0000-1000-8000-00805f9b34fb',
  // 	notifyUuid: '0000ff01-0000-1000-8000-00805f9b34fb'
  // },
)
```

#### 断开设备

```javascript
ble.disconnect()
```

#### 写入数据

- 写入字节数组

  ```javascript
  // 参数为字节数组，需设备支持的指令，这点很重要
  ble.writeBytes([0x10, 0xff, 0x30])
  ```

- 写入字符串

  ```javascript
  // 参数1: 字符串内容，传的字符串需设备支持的指令，这点很重要
  // 参数2(可选): 字符串编码, 默认 "UTF_8" 编码, 传 "GBK" 使用GBK编码
  ble.writeString('ABC')
  ble.writeString('ABC', 'GBK')
  ```

#### 读取特征值二进制数据值

```javascript
// serviceId 和 characteristicId 需要修改下，不一定是这个
ble.readCharacteristicValue({
	serviceId: '1111',
	characteristicId: '2222',
	callback: (resp) => {
		console.log(`读取${resp.isOK ? '成功' : '失败'}`);
		if (resp.isOK) {
			const data = resp.data;
			let result = String.fromCharCode(...data);
			console.log(`readCharacteristicValue收到数据: ${data}`)
			console.log(`readCharacteristicValue 数据转字符串: ${result}`);
		}
	}
});
```

#### 设置MTU分包大小

```javascript
ble.setMtu(100)
```

#### 读取已连接设备 RSSI 信号

```javascript
ble.readRssi((rssi) => {
	console.log('readRssi: ', rssi);
});
```


#### 蓝牙事件监听

##### 蓝牙数据回传监听

```javascript
ble.onDataReceived((data) => {
  let result = String.fromCharCode(...data);
  console.log(`收到蓝牙数据: ${data}`)
  console.log(`收到蓝牙数据转字符串: ${result}`);
});

```

##### 蓝牙状态监听

```javascript
ble.onBluetoothStatusChanged((isOn) => {
  uni.showToast({
    title: isOn ? "蓝牙已开启" : "蓝牙已关闭",
    icon: 'none'
  });
});
```

##### 蓝牙连接状态监听

```javascript
ble.onConnectionStateChange((connectionState) => {
  uni.showToast({
    title: connectionState.msg,
    icon: 'none'
  })
  switch (connectionState.state) {
    case 'connectFail':
				// 连接失败
      break;
    case 'disconnected':
				// 断开连接
      break;
    case 'connected':
      // 断开成功
      break;
  }
});
```

##### MTU设置回调

```javascript
ble.onMtuChanged((mtu, isOK) => {
	console.log("mtu设置" + (isOK ? '成功' : '失败'), mtu);
});
```


##### 数据写入是否完成监听

```javascript
ble.onWriteComplete((isComplete) => {
  console.log(isComplete ? "写入完成" : "写入失败");
})
```



#### 蓝牙权限

- **鸿蒙NEXT**

  ```html
  ohos.permission.ACCESS_BLUETOOTH
  ```

- **安卓**

  ```html
  <uses-permission android:name="android.permission.BLUETOOTH"/>
  <uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
  <uses-permission android:name="android.permission.BLUETOOTH_CONNECT"/>
  <uses-permission android:name="android.permission.BLUETOOTH_SCAN"/>
  <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION"/>
  <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION"/>
  <uses-permission android:name="android.permission.BLUETOOTH_SCAN" />
  ```

- **iOS**

  ```html
  <key>NSBluetoothAlwaysUsageDescription</key>
  <string>开启蓝牙</string>
  <key>NSBluetoothPeripheralUsageDescription</key>
  <string>蓝牙</string>
  <key>UIBackgroundModes</key>
  <array>
      <string>bluetooth-central</string>
  </array>
  ```



## 类型定义

### 设备信息

```typescript
interface BLEDevice {
  name: string       // 设备名称
  deviceId: string   // 设备唯一标识
  rssi: number       // 信号强度
  manufacturerData ?: string[]	// 制造商数据
}
```

### 连接配置

```typescript
interface ConnectOptions {
  serviceUuid: string   // 主服务UUID
  writeUuid: string     // 写入特征UUID
  notifyUuid: string    // 通知特征UUID
  autoConnect?: boolean // 是否自动重连
}
```

### 服务信息

```typescript
interface ServiceAndCharacteristics {
  service: string         // 服务UUID
  characteristics: string[] // 特征值列表
}
```

### 连接状态

```typescript
type ConnectionState = {
  status: 'connected' | 'disconnected' | 'failed'
  message?: string
}
```
