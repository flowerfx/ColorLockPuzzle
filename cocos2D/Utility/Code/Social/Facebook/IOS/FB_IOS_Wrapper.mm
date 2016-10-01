//
//  IAP_IOS_.m
//  CrystalNumberful
//
//  Created by qchien on 12/3/15.
//
//
#import "FB_ISO_Helper.h"
#import "FB_IOS_Wrapper.h"

namespace Utility
{
    namespace Social
    {
        namespace Facebook
        {
            
            void LoginFacebook()
            {
                [[FB_IOS_Helper defaultHelper] LoginFacebook];
               
            }
            
            int PostMessageToFacebook(RKString message_str , RKString url)
            {
                NSString * message_fb = [[NSString alloc]initWithBytes:(const void *)message_str.ToUnicode()
                                                                length:message_str.LengthUTF()
                                                                encoding:NSUTF32StringEncoding];
                
                NSString * url_fb = [NSString stringWithUTF8String:url.GetString()];
                
                return [[FB_IOS_Helper defaultHelper] PostMessageToFacebook:message_fb withURL:url_fb];
            }
            
            void GetFrofileFBWithAccessToken()
            {
                [[FB_IOS_Helper defaultHelper]GetFrofileFBWithAccessToken ];
                
            }
            
            RKString GetIDFacebook()
            {
                return [[FB_IOS_Helper defaultHelper]GetIDFacebook];
                
            }
            
            RKString GetNameFacebook()
            {
                return [[FB_IOS_Helper defaultHelper]GetNameFacebook];
            }
            
            RKString GetURLAvatarFBFromID(RKString user_id)
            {
                NSString * id_str = [NSString stringWithUTF8String:user_id.GetString()];
                return [[FB_IOS_Helper defaultHelper]GetURLAvatarFBFromID:id_str];
            }
            
        }
    }
}