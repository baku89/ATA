import c4d
import SimpleOSC as osc
from pync import Notifier

def onSaved(addr, tags, data, source):
    if IsRendering:
        print "saved: %d" % data[0]
        
        f = Time.GetFrame(24)
        
        if f != data[0]:
            return
    
        doc.SetTime( c4d.BaseTime( f + 1, 24 ) )
        c4d.DrawViews( c4d.DA_ONLY_ACTIVE_VIEW|c4d.DA_NO_THREAD|c4d.DA_NO_REDUCTION|c4d.DA_STATICBREAK )
        c4d.GeSyncMessage( c4d.EVMSG_TIMECHANGED )
        c4d.EventAdd( c4d.EVENT_ANIMATE )

def main():
    
    
    if (Time < StartTime or EndTime < Time):
        return
    
    opacity = 1.0 if Time == StartTime else Opacity
    frame = Time.GetFrame(24) if IsRendering else -1
    
    osc.initOSCClient(port=1234)
    osc.sendOSCMsg("/update", [frame, Speed / 100, opacity, Threshold, Offset, Suffix])
    
    if IsRendering:
        if Time == StartTime:
            "Start Rendering~~~~~~~~~"
            
        print "Rendering %d" % frame
        
        if EndTime == Time:
            doc.SearchObject("====OSC===")[c4d.ID_USERDATA,8] = False
            print "END!!"
            Notifier.notify("END!!!")
    
    if osc.server == 0:
        print "init osc server"
        osc.initOSCServer(port=4321)
        osc.startOSCServer()
    
    osc.setOSCHandler(address="/save-frame", hd=onSaved)
    