import e2studio

class DebuggerDownload(object):

    def LoadModule(
            self, fileName="", downloadOption=DownloadOption.Both,
            append=False, flashErase=False, vendorType=VendorType.Auto):
        try:
            if (fileName == ""):
                e2studio.download_loadmodule(debugger.debug_session)
                return True

            if (flashErase):
                raise Exception("Flash earse is not supported")

            load_image = True
            load_symbols = True
            if (downloadOption == DownloadOption.NoSymbol):
                load_symbols = False
            elif (downloadOption == DownloadOption.SymbolOnly):
                load_image = False

            e2studio.download_loadmodule(debugger.debug_session, fileName, 0, load_image, load_symbols, not append)
            return True
        except Exception as e:
            print(e)
            return False
