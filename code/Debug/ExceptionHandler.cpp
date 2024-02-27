#include <kamek.hpp>
#include <core/egg/Exception.hpp>
#include <core/rvl/ipc/ipc.hpp>
#include <core/rvl/base/ppc.hpp>
#include <core/rvl/devfs/isfs.hpp>
#include <core/rvl/os/OS.hpp>
#include <core/rvl/os/Error.hpp>
#include <core/rvl/os/thread.hpp>
#include <core/egg/Exception.hpp>
#include <core/nw4r/db/Exception.hpp>
#include <include/c_stdarg.h>
#include <Debug/Debug.hpp>
#include <core/rvl/vi.hpp>
#include <core/egg/Thread.hpp>
#include <core/rvl/wpad.h>
#include <game/Input/InputData.hpp>
#include <game/Input/InputData.hpp>
#include <game/UI/MenuData/MenuData.hpp>
#include <Controller/MiscController.hpp>
#include <core/System/SystemManager.hpp>
#include <Debug/SymbolMap.hpp>
#include <main.hpp>

extern char gameID[4];

namespace CosmosDebug{

    static char output[0x100];

    void HandlePanic(const char *file, int line, const char *fmt, va_list vlist, bool halt, u32 LR)
    {

        char format[0x100];
        snprintf(output, 0x100, "%s:%d Panic:\n", file, line);
        vsnprintf(format, 0x100, fmt, vlist);
        snprintf(output, 0x100, "%s%s", output, format);
        OSContext * context = OSGetCurrentContext();
        nw4r::db::ExceptionCallbackParam exc;
        exc.error = 0x30; 
        exc.context = context;
        exc.dar = 0x0;
        exc.dsisr = LR;
        #ifndef FORCEOSFATAL
        nw4r::db::DumpException_(&exc);
        #else
        u32 black = 0x000000FF;
        u32 white = 0xFFFFFFFF;

        OSFatal(&white, &black, output);
        #endif
    }
    
    kmWrite32(0x80026028, 0x60000000);
    kmWrite32(0x8002602C, 0x60000000);
    kmWrite32(0x80026038, 0x7c080378);
    kmCall(0x8002603c, HandlePanic);

    void HandleOSPanic(const char * file, u32 line, char * message)
    {
        snprintf(output, 0x100, "%s:%d Panic:\n%s", file, line, message);
        OSContext * context = OSGetCurrentContext();
        nw4r::db::ExceptionCallbackParam exc;
        exc.error = 0x31; 
        exc.context = context;
        exc.dar = (u32) output;
        exc.dsisr = 0x0;
        #ifndef FORCEOSFATAL
        nw4r::db::DumpException_(&exc);
        #else
        u32 black = 0x000000FF;
        u32 white = 0xFFFFFFFF;

        OSFatal(&white, &black, output);
        #endif
    }

    kmBranch(0x801a2660, HandleOSPanic);

    char filepath[20];

    char * GetRegionName()
    {
        switch (gameID[3])
        {
            default:
            case 'P':
                return "PAL";
                break;
            case 'E':
                return "NTSC-U";
                break;
            case 'J':
                return "NTSC-J";
                break;
        }

        return filepath;
    }
    extern u32 SRR0_addr;
    /*
    void PrintUnhandled()
    {
        char text[] = "Dearest Player\nI hope it finds you well.\nWe seem to have found ourselves a crash in the game.\nPlease consider taking a screenshot and sending\n it to #bug-reports\n\n";
        char final[200];
        snprintf(final, 200, text);

        u32 black = 0;
        u32 white = ~0;
        OSFatal(&white,&black,final);

    }

    kmBranch(0x801a2ce8, PrintUnhandled);
    */

    void PrintHeader()
    {
        nw4r::db::Exception_Printf_("**** COSMOS EXCEPTION HANDLER ****\n");
        nw4r::db::Exception_Printf_("Platform: %s\nCosmos %s (%s, %s %s)\n-------------------------------\n", CosmosDebug::GetPlatformString(), __COSMOS_VERSION__, __COMPILER_VERSION__, __DATE__, __TIME__);
        
        u32 tick0 = OSGetTick() & 0x3;

        /*
        if(tick0 == 0){
            nw4r::db::Exception_Printf_("*** Dearest Player\n*** I hope it finds you well. We seem to have\n*** found ourselves a crash in the game. Please\n*** consider taking a screenshot and sending it \n*** to #bug-reports\n");
            nw4r::db::Exception_Printf_("***\n");
            nw4r::db::Exception_Printf_("*** Sincerely, @VolcanoPiece1\n");
        }
        else if(tick0 == 1)
        {
            nw4r::db::Exception_Printf_("*** Lorem ipsum dolor sit amet,\n*** consectetur adipiscing elit.\n*** Nunc ipsum dui, aliquam in volutpat a,\n*** feugiat et justo.\n*** Duis commodo varius ex,\n*** ut viverra risus malesuada consectetur.");
        }
        */
        return;
    }

    kmCall(0x800236c8, PrintHeader);

    void PrintSRR(const char * format, u32 srr0, u32 srr1)
    {
        nw4r::db::Exception_Printf_("SRR0:   %08XH   SRR1:%08XH\n", srr0, srr1);
        nw4r::db::Exception_Printf_("%s\n",SymbolManager::GetSymbolName(srr0));

    }
    kmCall(0x80023b8c, PrintSRR);

    void PrintStack(const char * format, u32 curStack, u32 nextStack, u32 curValue)
    {
        nw4r::db::Exception_Printf_("%08x:  %08x    %08x: %s",curStack,nextStack,curValue,SymbolManager::GetSymbolName(curValue));
    }

    kmCall(0x80023930, PrintStack);
    

    void PrintPanic(u16 error, const OSContext * context, u32 dsisr, u32 dar)
    {
        char * region_name= GetRegionName();

        if(error == 0x30)
            nw4r::db::Exception_Printf_("\n\n*** COSMOS PANIC HANDLER ***\nnw4r::Panic() has been called at 0x%08x (%s)\n<Symbol not found>\n\n", dsisr-4, region_name);
        else if(error == 0x31)
            nw4r::db::Exception_Printf_("\n\n*** COSMOS PANIC HANDLER ***\nRVL::OSPanic() has been called; (%s)\n<Symbol not found>\n\n", region_name);
        else if(error == 0x32)
            nw4r::db::Exception_Printf_("\n\n*** COSMOS PANIC HANDLER ***\nCosmos::Panic() has been called; (%s)\n<Symbol not found>\n\n", region_name);
        nw4r::db::Exception_Printf_("Platform: %s\nCosmos %s (%s %s)\n-------------------------------\n", CosmosDebug::GetPlatformString(), __COSMOS_VERSION__, __DATE__, __TIME__);
        nw4r::db::Exception_Printf_("*** Message ***\n%s\n", (char *)dar );
    }


    #ifdef FORCEOSFATAL

    char output2[0x100];
    void PrintFATALContext(u16 error, const OSContext * context, u32 dsisr, u32 dar)
    {
        snprintf(output2, 0x100, "AN ERROR HAS OCCURED at %8x: ", context->srr0);

        u32 black = 0x000000FF;
        u32 white = 0xFFFFFFFF;

        OSFatal(&white, &black, output2);
    }
    
    kmCall(0x80023484, PrintFATALContext);

    #else
    

    void PrintContext(u16 error, const OSContext * context, u32 dsisr, u32 dar)
    {
        if(error < 0x30)
            nw4r::db::PrintContext_(error, context, dsisr, dar);
        else
            PrintPanic(error, context, dsisr, dar);
        return;
    }

    kmCall(0x80023484, PrintContext);

    #endif

    void SetConsoleParams(){
        nw4r::db::detail::ConsoleHead *console = EGG::Exception::console;
        console->width = 0x70;
        console->height = 0x40;
    }
    BootHook ConsoleParams(SetConsoleParams, LOW);

    bool ExceptionCallBack_(nw4r::db::detail::ConsoleHead * head, void * )
    {
        s32 scrollCooldown = 200;

        OSReport("CALLBACK...\n");

        if(head == nullptr)
        {
            CosmosError("No Console Found\n");
            return false;
        }
        VISetBlack(0);
        OSReport("cancel all thread...\n");
        EGG::Thread::kandoTestCancelAllThread();
        OSReport("done\n");
        
        OSDisableInterrupts();
        OSDisableScheduler();
        OSEnableInterrupts();

        s32 lineCount = head->ringTopLineCnt;
        s32 totalLineCount = head->GetLineCount();

        head->viewTopLine = lineCount;
        head->isVisible = true;
        head->DrawDirect();

        
        u32 controllerType;
        s32 ret = WPADProbe(0, &controllerType);

        bool ccp = (ret == 0 && (controllerType == 2 || controllerType == 7));
        
        u32 controller = MenuData::sInstance->pad.padInfos[0].controllerSlotAndTypeActive;
        ControllerType type = ControllerType(controller & 0xFF);
        RealControllerHolder * holder = &InputData::sInstance->realControllerHolders[0];

        bool lock = true;

        while(true)
        {
            KPADStatus wStatus;
            PADStatus gcStatus[4];
            WPADCLStatus clStatus;

            KPADRead(0, &wStatus, 1);
            PADRead(&gcStatus);
            
            if(ccp)
            {
                KPADGetUnifiedWpadStatus(0, &clStatus, 1);
            }
            holder->inputStates[1] = holder->inputStates[0];
            switch(type)
            {
                case(CLASSIC):
                    holder->inputStates[0].buttonRaw = clStatus.buttons;
                    break;
                case(NUNCHUCK):
                case(WHEEL):
                    holder->inputStates[0].buttonRaw = wStatus.buttons;
                    break;
                case(GCN):
                    holder->inputStates[0].buttonRaw = gcStatus[0].buttons;
                default:
                    break;
            }

            if(SystemManager::sInstance->doShutDown > 0)
            {
                Cosmos::System::Shutdown(true);
            }

            if(CosmosController::isPressed(holder, type, CosmosController::BUTTON_HOME) || CosmosController::isPressed(holder, type, CosmosController::BUTTON_PLUS))
            {
                Cosmos::System::Shutdown(true);
            }


            u32 tick0 = OSGetTick();
            u32 tick1 = tick0;
            while (OSTicksToMilliseconds(tick1-tick0) < scrollCooldown) tick1 = OSGetTick();

            s32 xPos = head->viewPosX;
            s32 currentTopLine = head->viewTopLine;

            s32 prevXPos = xPos;
            s32 prevTopLine = currentTopLine;

            bool down = CosmosController::isPressed(holder, type, CosmosController::BUTTON_DPAD_DOWN);
            bool up = CosmosController::isPressed(holder, type, CosmosController::BUTTON_DPAD_UP);
            bool left = CosmosController::isPressed(holder, type, CosmosController::BUTTON_DPAD_LEFT);
            bool right = CosmosController::isPressed(holder, type, CosmosController::BUTTON_DPAD_RIGHT);

            if(lock)
            {
                currentTopLine++;
                if(currentTopLine == totalLineCount - (head->viewLines / 2))
                {
                    lock = false;
                    scrollCooldown = 100;
                    currentTopLine = 0;
                }
            }
            else
            {
                if(right)
                    xPos = Max(xPos - 5, -130);
                else if(left)
                    xPos = Min(xPos + 5, 10);

                if(down)
                    currentTopLine = Min(currentTopLine + 1, totalLineCount - head->viewLines);
                else if(up)
                    currentTopLine = Max(currentTopLine - 1, lineCount);
            }

            if (currentTopLine != prevTopLine || xPos != prevXPos) {
                head->viewPosX = xPos;
                head->viewTopLine = currentTopLine;
                head->DrawDirect();
            }
        }
    }
    kmBranch(0x80226464 ,ExceptionCallBack_);

}