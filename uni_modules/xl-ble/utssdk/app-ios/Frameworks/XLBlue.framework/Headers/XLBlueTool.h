//
//  XLBlueTool.h
//  XLBlue
//
//  Created by mac on 7/30/25.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

@protocol XLBlueToolDelegate <NSObject>

@optional
- (void)deviceDidDiscover:(NSDictionary *_Nonnull)dict;
- (void)connectStateDidChange:(NSDictionary *_Nonnull)dict;
- (void)onReadReceive:(NSDictionary *_Nullable)data;
- (void)onReceive:(NSData *_Nullable)data;
- (void)blueStateChange:(BOOL)isOn;
- (void)onWriteComplete:(BOOL)isComplete;
- (void)onMtuChanged:(int)mtu;
- (void)onRssiChanged:(NSString *_Nullable)mtu;

@end


@interface XLBlueTool : NSObject


@property (nonatomic, weak) id<XLBlueToolDelegate> delegate;

- (BOOL)isConnect;

- (void)openAppSettings;

- (void)blueState;

- (NSDictionary *_Nullable)getConnectedDevice;

- (NSArray *_Nullable)getServiceAndCharacteristics;

- (NSNumber *_Nullable)getMaxMtu;

- (void)configure:(NSString *_Nonnull)str;

- (void)datagramSize:(int)size;

- (void)configureUUID:(NSString *_Nonnull)str;

- (void)startScan:(NSString *_Nonnull)str;

- (void)stopScan;

- (void)connect:(NSString *_Nullable)deviceId;

- (void)disconnect;

- (void)writeBytes:(NSString *_Nonnull)str;

- (void)writeStringCmd:(NSString *_Nonnull)str;

- (void)read:(NSString *_Nonnull)str;

- (void)readRssi;

@end

