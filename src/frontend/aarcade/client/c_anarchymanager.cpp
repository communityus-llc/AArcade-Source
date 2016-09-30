#include "cbase.h"
#include "aa_globals.h"
#include "c_anarchymanager.h"
//#include "c_systemtime.h"
#include "WinBase.h"
#include <cctype>
#include <algorithm>
#include "c_browseslate.h"
#include "../../sqlite/include/sqlite/sqlite3.h"
//#include "mathlib/mathlib.h"
//#include <math.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

C_AnarchyManager g_AnarchyManager;
extern C_AnarchyManager* g_pAnarchyManager(&g_AnarchyManager);

C_AnarchyManager::C_AnarchyManager() : CAutoGameSystemPerFrame("C_AnarchyManager")
{
	DevMsg("AnarchyManager: Constructor\n");
	m_state = AASTATE_NONE;
	m_iState = 0;
	m_bIncrementState = false;
	m_bPaused = false;
	m_pCanvasManager = null;
	//m_pWebManager = null;
	//m_pLoadingManager = null;
	m_pLibretroManager = null;
	m_pSteamBrowserManager = null;
	m_pInputManager = null;
	m_pSelectedEntity = null;
	m_pMountManager = null;
	m_pInstanceManager = null;
	m_dLastGenerateIdTime = 0;
	m_lastGeneratedChars = "000000000000";
}

C_AnarchyManager::~C_AnarchyManager()
{
	DevMsg("AnarchyManager: Destructor\n");
}

bool C_AnarchyManager::Init()
{
	DevMsg("AnarchyManager: Init\n");
	return true;
}

void C_AnarchyManager::PostInit()
{
	DevMsg("AnarchyManager: PostInit\n");
}

void C_AnarchyManager::Shutdown()
{
	DevMsg("AnarchyManager: Shutdown\n");

	/*
	if (m_pWebManager)
	{
		delete m_pWebManager;
		m_pWebManager = null;
	}
	*/

//	delete m_pLoadingManager;
	//m_pLoadingManager = null;

	if (m_pLibretroManager)
	{
		delete m_pLibretroManager;
		m_pLibretroManager = null;
	}

	if (m_pSteamBrowserManager)
	{
		delete m_pSteamBrowserManager;
		m_pSteamBrowserManager = null;
	}

	if (m_pMountManager)
	{
		delete m_pMountManager;
		m_pMountManager = null;
	}

	if (m_pWorkshopManager)
	{
		delete m_pWorkshopManager;
		m_pWorkshopManager = null;
	}

	if (m_pMetaverseManager)
	{
		delete m_pMetaverseManager;
		m_pMetaverseManager = null;
	}

	if (m_pInstanceManager)
	{
		delete m_pInstanceManager;
		m_pInstanceManager = null;
	}

	if (m_pAwesomiumBrowserManager)
	{
		delete m_pAwesomiumBrowserManager;
		m_pAwesomiumBrowserManager = null;
	}

	if (m_pInputManager)
	{
		m_pInputManager->DeactivateInputMode(true);
		delete m_pInputManager;
		m_pInputManager = null;
	}

	DevMsg("AnarchyManager: Finished Shutdown\n");

	//g_pFullFileSystem->RemoveAllSearchPaths();	// doesn't make shutdown faster and causes warnings about failing to write cfg/server_blacklist.txt
}

void C_AnarchyManager::LevelInitPreEntity()
{
	DevMsg("AnarchyManager: LevelInitPreEntity\n");
	m_instanceId = m_nextInstanceId;
}

void C_AnarchyManager::LevelInitPostEntity()
{
	DevMsg("AnarchyManager: LevelInitPostEntity\n");

	if (m_instanceId != "")
		g_pAnarchyManager->GetInstanceManager()->LoadLegacyInstance(m_instanceId);
}

void C_AnarchyManager::LevelShutdownPreClearSteamAPIContext()
{
	DevMsg("AnarchyManager: LevelShutdownPreClearSteamAPIContext\n");
}

void C_AnarchyManager::LevelShutdownPreEntity()
{
	DevMsg("AnarchyManager: LevelShutdownPreEntity\n");
	C_BaseEntity* pEntity = this->GetSelectedEntity();
	if (pEntity)
		this->DeselectEntity(pEntity);

	m_pCanvasManager->LevelShutdownPreEntity();

	/*
	C_WebTab* pWebTab = m_pWebManager->GetSelectedWebTab();
	if (pWebTab)
		m_pWebManager->DeselectWebTab(pWebTab);

	m_pWebManager->LevelShutdownPreEntity();
	*/
}

void C_AnarchyManager::LevelShutdownPostEntity()
{
	DevMsg("AnarchyManager: LevelShutdownPostEntity\n");
	m_instanceId = "";	// wtf is this??

	// Clear out the simple images
}

void C_AnarchyManager::OnSave()
{
	DevMsg("AnarchyManager: OnSave\n");
}

void C_AnarchyManager::OnRestore()
{
	DevMsg("AnarchyManager: OnRestore\n");
}
void C_AnarchyManager::SafeRemoveIfDesired()
{
	//DevMsg("AnarchyManager: SafeRemoveIfDesired\n");
}

bool C_AnarchyManager::IsPerFrame()
{
	DevMsg("AnarchyManager: IsPerFrame\n");
	return true;
}

void C_AnarchyManager::PreRender()
{
	//DevMsg("AnarchyManager: PreRender\n");
}

void C_AnarchyManager::IncrementState()
{
	m_bIncrementState = true;
}

void C_AnarchyManager::Update(float frametime)
{
	if (m_bIncrementState)
	{
		m_bIncrementState = false;

		switch (m_state)
		{
			case AASTATE_NONE:
				m_state = AASTATE_INPUTMANAGER;
				break;

			case AASTATE_INPUTMANAGER:
				m_state = AASTATE_CANVASMANAGER;
				break;

			case AASTATE_CANVASMANAGER:
				m_state = AASTATE_LIBRETROMANAGER;
				//m_state = AASTATE_STEAMBROWSERMANAGER;
				break;

			case AASTATE_LIBRETROMANAGER:
			{
				// TEST: AUTO-CREATE AN INSTANCE, LOAD THE FFMPEG CORE, AND PLAY A MOVIE
				/*
				C_LibretroInstance* pLibretroInstance = m_pLibretroManager->CreateLibretroInstance();//>GetSelectedLibretroInstance();// 
				pLibretroInstance->Init();

				// load a core
				pLibretroInstance->LoadCore();

				// load a file
				pLibretroInstance->LoadGame();

				// tell the input manager that the libretro instance is active
				C_InputListenerLibretro* pListener = m_pLibretroManager->GetInputListener();
				m_pInputManager->SetInputCanvasTexture(pLibretroInstance->GetTexture());
				m_pInputManager->ActivateInputMode(true, true, (C_InputListener*)pListener);
				*/

				m_state = AASTATE_STEAMBROWSERMANAGER;
				break;
			}

			case AASTATE_STEAMBROWSERMANAGER:
			{
				// TEST: AUTO-CREATE AN INSTANCE, LOAD A WEBSITE
				/*
				C_SteamBrowserInstance* pSteamBrowserInstance = m_pSteamBrowserManager->CreateSteamBrowserInstance();
				pSteamBrowserInstance->Init("", "http://smarcade.net/dlcv2/view_youtube.php?id=CmRih_VtVAs&autoplay=1", null);//https://www.youtube.com/html5
				*/

				m_state = AASTATE_AWESOMIUMBROWSERMANAGER;
				//m_state = AASTATE_RUN;
				break;
			}

			case AASTATE_AWESOMIUMBROWSERMANAGER:
				m_state = AASTATE_AWESOMIUMBROWSERMANAGERWAIT;
				break;

			case AASTATE_AWESOMIUMBROWSERMANAGERWAIT:
				m_state = AASTATE_AWESOMIUMBROWSERMANAGERHUD;
				break;

			case AASTATE_AWESOMIUMBROWSERMANAGERHUD:
				m_state = AASTATE_AWESOMIUMBROWSERMANAGERHUDWAIT;
				break;

			case AASTATE_AWESOMIUMBROWSERMANAGERHUDWAIT:
				m_state = AASTATE_AWESOMIUMBROWSERMANAGERHUDINIT;
				break;

			case AASTATE_AWESOMIUMBROWSERMANAGERHUDINIT:
				m_state = AASTATE_AWESOMIUMBROWSERMANAGERIMAGES;
				break;

			case AASTATE_AWESOMIUMBROWSERMANAGERIMAGES:
				m_state = AASTATE_AWESOMIUMBROWSERMANAGERIMAGESWAIT;
				break;

			case AASTATE_AWESOMIUMBROWSERMANAGERIMAGESWAIT:
				m_state = AASTATE_AWESOMIUMBROWSERMANAGERIMAGESINIT;
				break;

			case AASTATE_AWESOMIUMBROWSERMANAGERIMAGESINIT:
				m_state = AASTATE_RUN;
				break;
		}
	}

	switch (m_state)
	{
		case AASTATE_RUN:
			if (m_bPaused)	// FIXME: You might want to let the web manager do its core logic, but don't render anything.
				return;

			//DevMsg("Float: %f\n", frametime);	// deltatime
			//DevMsg("Float: %i\n", gpGlobals->framecount);	// numframes total

			if (m_pCanvasManager)
				m_pCanvasManager->Update();

			if (m_pLibretroManager)
				m_pLibretroManager->Update();

			if (m_pSteamBrowserManager)
				m_pSteamBrowserManager->Update();

			if (m_pAwesomiumBrowserManager)
				m_pAwesomiumBrowserManager->Update();
			/*
			if (m_pWebManager)
				m_pWebManager->Update();
			*/

			//DevMsg("AnarchyManager: Update\n");
			break;

		case AASTATE_INPUTMANAGER:
			m_pInputManager = new C_InputManager();	// then wait for state change
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_CANVASMANAGER:
			m_pCanvasManager = new C_CanvasManager();	// then wait for state change
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_LIBRETROMANAGER:
			m_pLibretroManager = new C_LibretroManager();
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_STEAMBROWSERMANAGER:
			m_pSteamBrowserManager = new C_SteamBrowserManager();
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGER:
			m_pAwesomiumBrowserManager = new C_AwesomiumBrowserManager();
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGERWAIT:
			if (m_pCanvasManager)
				m_pCanvasManager->Update();

			m_pAwesomiumBrowserManager->Update();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGERHUD:
			//m_pAwesomiumBrowserManager->CreateAwesomiumBrowserInstance("hud", "asset://ui/startup.html", true);	// defaults to asset://ui/blank.html
			m_pAwesomiumBrowserManager->CreateAwesomiumBrowserInstance("hud", "asset://ui/blank.html", true);
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGERHUDWAIT:
			if (m_pCanvasManager)
				m_pCanvasManager->Update();

			m_pAwesomiumBrowserManager->Update();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGERHUDINIT:
			DevMsg("Finished initing HUD.\n");
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGERIMAGES:
			m_pAwesomiumBrowserManager->CreateAwesomiumBrowserInstance("images", "asset://ui/imageLoader.html", true);	// defaults to asset://ui/blank.html
			g_pAnarchyManager->IncrementState();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGERIMAGESWAIT:
			if (m_pCanvasManager)
				m_pCanvasManager->Update();

			m_pAwesomiumBrowserManager->Update();
			break;

		case AASTATE_AWESOMIUMBROWSERMANAGERIMAGESINIT:
			DevMsg("Finished initing IMAGES.\n");
			m_pInstanceManager = new C_InstanceManager();
			m_pMetaverseManager = new C_MetaverseManager();
			m_pInputManager = new C_InputManager();

			/*
			C_AwesomiumBrowserInstance* pHudBrowserInstance = m_pAwesomiumBrowserManager->FindAwesomiumBrowserInstance("hud");
			//pAwesomiumBrowserInstance->SetUrl("asset://ui/startup.html");
			

			// Now start loading stuff in...
			//C_WebTab* pHudWebTab = m_pWebManager->GetHudWebTab();
			//C_EmbeddedInstance* pEmbeddedInstance = m_p
			m_pAwesomiumBrowserManager->SelectAwesomiumBrowserInstance(pHudBrowserInstance);
			m_pInputManager->ActivateInputMode(true, true, pHudBrowserInstance);
			//g_pAnarchyManager->GetInputManager()->ActivateInputMode(true);

			unsigned int uCount;
			std::string num;

			// And continue starting up
			uCount = m_pMetaverseManager->LoadAllLocalTypes();
			num = VarArgs("%u", uCount);
			pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Loading Types", "locallibrarytypes", "0", num, num);

			//= m_pMetaverseManager->LoadAllLocalTypes();
			//std::string num = VarArgs("%u", uItemCount);
			//	pHudWebTab->AddHudLoadingMessage("progress", "", "Loading Types", "locallibrarytypes", "0", num, num);

			uCount = m_pMetaverseManager->LoadAllLocalModels();
			num = VarArgs("%u", uCount);
			pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Loading Models", "locallibrarymodels", "0", num, num);

			//uItemCount = m_pMetaverseManager->LoadAllLocalApps();

			// load ALL local apps
			KeyValues* app = m_pMetaverseManager->LoadFirstLocalApp("MOD");
			if (app)
				pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Loading Apps", "locallibraryapps", "", "", "+", "loadNextLocalAppCallback");
			else
				this->OnLoadAllLocalAppsComplete();
			*/

			this->IncrementState();
			break;

		//case AASTATE_WEBMANAGER:
			/*
			// create a libretro instance
			C_LibretroInstance* pLibretroInstance = m_pLibretroManager->CreateLibretroInstance();//>GetSelectedLibretroInstance();// 
			pLibretroInstance->Init();

			// load a core
			pLibretroInstance->LoadCore();

			// load a file
			pLibretroInstance->LoadGame();

			// tell the input manager that the libretro instance is active
			C_InputListenerLibretro* pListener = m_pLibretroManager->GetInputListener();
			m_pInputManager->SetInputCanvasTexture(pLibretroInstance->GetTexture());
			m_pInputManager->ActivateInputMode(true, true, (C_InputListener*)pListener);

			steamapicontext->SteamHTMLSurface()->Init();
			SteamAPICall_t hAPICall = steamapicontext->SteamHTMLSurface()->CreateBrowser("", "");
			C_SteamworksBrowser* pSteamworksBrowser = new C_SteamworksBrowser();
			pSteamworksBrowser->Init(hAPICall);

			this->SetState(AASTATE_RUN);
			*/
		//	break;
	}
}

#include "ienginevgui.h"
bool C_AnarchyManager::HandleUiToggle()
{
	// handle escape if in pause mode (ignore it)
	if (!engine->IsInGame())
	{
		// GOOD MAIN MENU EMBEDDED APP ESCAPE BINDS AS OF 9/13/2016

		if (m_pSteamBrowserManager)
		{
			C_SteamBrowserInstance* pInstance = m_pSteamBrowserManager->GetSelectedSteamBrowserInstance();
			//if (m_pInputManager->GetMainMenuMode() && m_pInputManager->GetInputMode() && m_pInputManager->GetFullscreenMode() && pInstance && pInstance->GetTexture() && pInstance->GetTexture() == m_pInputManager->GetInputCanvasTexture())
			if (g_pAnarchyManager->GetInputManager()->GetEmbeddedInstance() == pInstance)
			{
				m_pSteamBrowserManager->DestroySteamBrowserInstance(pInstance);
				m_pInputManager->SetEmbeddedInstance(null);
				return true;
			}
		}

		if (m_pLibretroManager)
		{
			C_LibretroInstance* pInstance = m_pLibretroManager->GetSelectedLibretroInstance();
			//if (m_pInputManager->GetMainMenuMode() && m_pInputManager->GetInputMode() && m_pInputManager->GetFullscreenMode() && pInstance && pInstance->GetTexture() && pInstance->GetTexture() == m_pInputManager->GetInputCanvasTexture())
			if (g_pAnarchyManager->GetInputManager()->GetEmbeddedInstance() == pInstance)
			{
				m_pLibretroManager->DestroyLibretroInstance(pInstance);
				m_pInputManager->SetEmbeddedInstance(null);
				return true;
			}
		}

		if (m_pAwesomiumBrowserManager)
		{
			C_AwesomiumBrowserInstance* pInstance = m_pAwesomiumBrowserManager->GetSelectedAwesomiumBrowserInstance();
			//if (m_pInputManager->GetMainMenuMode() && m_pInputManager->GetInputMode() && m_pInputManager->GetFullscreenMode() && pInstance && pInstance->GetTexture() && pInstance->GetTexture() == m_pInputManager->GetInputCanvasTexture())
			if (g_pAnarchyManager->GetInputManager()->GetEmbeddedInstance() == pInstance)
			{
				m_pAwesomiumBrowserManager->DestroyAwesomiumBrowserInstance(pInstance);
				m_pInputManager->SetEmbeddedInstance(null);
				return true;
			}
		}

		return false;
	}

	if (m_bPaused)
		return true;

	if (m_pInputManager->GetInputMode())
	{
		// handle escape if in fullscreen input mode (drop out of fullscreen mode)
		if ((!m_pInputManager->GetFullscreenMode() || !this->GetSelectedEntity() || m_pInputManager->GetWasForceInputMode()) || (this->GetSelectedEntity() && m_pInputManager->GetFullscreenMode()))
			m_pInputManager->DeactivateInputMode(true);
		else
			m_pInputManager->SetFullscreenMode(false);

		return true;
	}
	else if (!m_pInputManager->GetInputMode() && engine->IsInGame() )
	{
		// handle escape if not in input mode & map is loaded (display the main menu)
		//engine->IsInGame()
		//engine->IsPaused()
		if (!enginevgui->IsGameUIVisible())
		{
			C_AwesomiumBrowserInstance* pHudBrowserInstance = m_pAwesomiumBrowserManager->FindAwesomiumBrowserInstance("hud");

			//DevMsg("DISPLAY MAIN MENU\n");
			if (m_pSelectedEntity)
				this->DeselectEntity(m_pSelectedEntity, "asset://ui/welcome.html");
			else
				pHudBrowserInstance->SetUrl("asset://ui/welcome.html");

			m_pInputManager->ActivateInputMode(true, true);
			return false;
		}
	}

	return false;
}

void C_AnarchyManager::Pause()
{
	m_bPaused = true;
}

void C_AnarchyManager::Unpause()
{
	m_bPaused = false;
	m_pAwesomiumBrowserManager->FindAwesomiumBrowserInstance("hud")->SetUrl("asset://ui/blank.html");
//	m_pWebManager->GetHudWebTab()->SetUrl("asset://ui/blank.html");
	m_pInputManager->DeactivateInputMode(true);
}

void C_AnarchyManager::RunAArcade()
{
	C_AwesomiumBrowserInstance* pHudBrowserInstance = m_pAwesomiumBrowserManager->FindAwesomiumBrowserInstance("hud");
	//pAwesomiumBrowserInstance->SetUrl("asset://ui/startup.html");
//	pHudBrowserInstance->A
	pHudBrowserInstance->SetUrl("asset://ui/startup.html");
}

void C_AnarchyManager::PostRender()
{
	//DevMsg("AnarchyManager: PostRender\n");
}

// Over 15x faster than: (int)floor(value)
/*
inline int Floor2Int(float a)
{
	int RetVal;
#if defined( __i386__ )
	// Convert to int and back, compare, subtract one if too big
	__m128 a128 = _mm_set_ss(a);
	RetVal = _mm_cvtss_si32(a128);
	__m128 rounded128 = _mm_cvt_si2ss(_mm_setzero_ps(), RetVal);
	RetVal -= _mm_comigt_ss(rounded128, a128);
#else
	RetVal = static_cast<int>(floor(a));
#endif
	return RetVal;
}
*/
#include <chrono>
std::string C_AnarchyManager::GenerateUniqueId()
{
	/*
	DevMsg("AnarchyManager: GenerateUniqueId\n");

	// pseudo random pseudo unique ids until the firebase id generator can be ported to C++
	std::string id = "random";

	id += std::to_string(random->RandomInt(0, 10));
	id += std::to_string(random->RandomInt(0, 10));
	id += std::to_string(random->RandomInt(0, 10));
	id += std::to_string(random->RandomInt(0, 10));

	return id;
	*/

	std::string PUSH_CHARS = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
	
	//double now = vgui::system()->GetCurrentTime();
	//using namespace std::chrono;
	double now = std::chrono::system_clock::now().time_since_epoch().count();//GetCurrentTime();// vgui::system()->GetCurrentTime();
	now = floor(now / 64.0);
	now = floor(now / 64.0);

	//DevMsg("Time now: %lf\n", now);

	bool duplicateTime = (now == m_dLastGenerateIdTime);
	m_dLastGenerateIdTime = now;

	//char* timeStampChars[8];
	std::string timeStampChars = "00000000";
	for (unsigned int i = 8; i > 0; i--)
	{
		timeStampChars.replace(i-1, 1, 1, PUSH_CHARS.at(fmod(now, 64.0)));
		// NOTE: Can't use << here because javascript will convert to int and lose the upper bits.
//		if (now >= 64.0)
			now = floor(now / 64.0);
	//	else
		//	now = 0;
	}

	if (now != 0)
	{
		DevMsg("ERROR: We should have converted the entire timestamp. %f\n", now);
	}

	std::string id = timeStampChars;
	//bool bCharsExist = (m_lastGeneratedChars != "");
	if (!duplicateTime)
	{
		for (unsigned int i = 0; i < 12; i++)
		{
//			if (bCharsExist)
				m_lastGeneratedChars.replace(i, 1, 1, (char)floor(random->RandomFloat() * 64.0L));
	//		else
		//		m_lastGeneratedChars += VarArgs("%c", (char)floor(random->RandomFloat() * 64.0L));
		}
	}
	else
	{
		// If the timestamp hasn't changed since last push, use the same random number, except incremented by 1.
		unsigned int i;
		for (i = 11; i >= 0 && m_lastGeneratedChars.at(i) == 63; i--)
			m_lastGeneratedChars.replace(i, 1, 1, (char)0);

		m_lastGeneratedChars.replace(i, 1, 1, (char)(m_lastGeneratedChars.at(i) + 1));
	}

	for (unsigned int i = 0; i < 12; i++)
	{
		id += PUSH_CHARS.at(m_lastGeneratedChars.at(i));
	}

	if (id.length() != 20)
		DevMsg("ERROR: Lngth should be 20.\n");

	return id;
}

std::string C_AnarchyManager::ExtractLegacyId(std::string itemFile, KeyValues* item)
{
	std::string alphabet = "0123456789abcdef";

	std::string nameSnip = "";
	bool bPassed = true;

	size_t found = itemFile.find(":");
	if (found != std::string::npos)
		bPassed = false;

	if (bPassed)
	{
		found = itemFile.find_last_of("/\\");
		if (found == std::string::npos)
			bPassed = false;
	}

	if (bPassed)
	{
		nameSnip = itemFile.substr(found + 1);
		found = nameSnip.find_first_of(".");

		if (found == std::string::npos)
			bPassed = false;
		else
		{
			nameSnip = nameSnip.substr(0, found);

			unsigned int nameSnipLength = nameSnip.length();
			if (nameSnipLength != 8)
				bPassed = false;
			else
			{
				unsigned int i;
				for (i = 0; i < nameSnipLength; i++)
				{
					found = alphabet.find(nameSnip[i]);
					if (found == std::string::npos)
					{
						bPassed = false;
						break;
					}
				}
			}
		}
	}
	
	// generate a legacy ID based on the filelocation if given an item to work with
	if (!bPassed && item)
		nameSnip = this->GenerateLegacyHash(item->GetString("filelocation"));

	if (!bPassed)
		nameSnip = "";

	return nameSnip;
}

const char* C_AnarchyManager::GenerateLegacyHash(const char* text)
{
	char input[AA_MAX_STRING];
	Q_strcpy(input, text);

	// Convert it to lowercase & change all slashes to back-slashes
	V_FixSlashes(input);
	for (int i = 0; input[i] != '\0'; i++)
		input[i] = tolower(input[i]);

	char lower[256];
	unsigned m_crc = 0xffffffff;

	int inputLength = strlen(input);
	for (int i = 0; i < inputLength; i++)
	{
		lower[i] = tolower(input[i]);
	}

	for (int i = 0; i < inputLength; i++)
	{
		unsigned c = lower[i];
		m_crc ^= (c << 24);

		for (int j = 0; j < 8; j++)
		{
			const unsigned FLAG = 0x80000000;
			if ((m_crc & FLAG) == FLAG)
			{
				m_crc = (m_crc << 1) ^ 0x04C11DB7;
			}
			else
			{
				m_crc <<= 1;
			}
		}
	}

	return VarArgs("%08x", m_crc);
}

/*
var PUSH_CHARS = '-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz';

var now = new Date().getTime();
var duplicateTime = (now === this.lastPushTime);
this.lastPushTime = now;

var timeStampChars = new Array(8);
for (var i = 7; i >= 0; i--) {
timeStampChars[i] = PUSH_CHARS.charAt(now % 64);
// NOTE: Can't use << here because javascript will convert to int and lose the upper bits.
now = Math.floor(now / 64);
}
if (now !== 0) throw new Error('We should have converted the entire timestamp.');

var id = timeStampChars.join('');

if (!duplicateTime) {
for (i = 0; i < 12; i++) {
this.lastRandChars[i] = Math.floor(Math.random() * 64);
}
} else {
// If the timestamp hasn't changed since last push, use the same random number, except incremented by 1.
for (i = 11; i >= 0 && this.lastRandChars[i] === 63; i--) {
this.lastRandChars[i] = 0;
}
this.lastRandChars[i]++;
}
for (i = 0; i < 12; i++) {
id += PUSH_CHARS.charAt(this.lastRandChars[i]);
}
if(id.length != 20) throw new Error('Length should be 20.');

return id;
*/

void C_AnarchyManager::AnarchyStartup()
{
	DevMsg("AnarchyManager: AnarchyStartup\n");
	m_bIncrementState = true;
	//m_state = AASTATE_INPUTMANAGER;
	/*
	m_pInstanceManager = new C_InstanceManager();
	m_pMetaverseManager = new C_MetaverseManager();
	m_pInputManager = new C_InputManager();
	m_pWebManager = new C_WebManager();
	m_pWebManager->Init();
	*/
}

/*
void C_AnarchyManager::OnWebManagerReady()
{
	C_WebTab* pHudWebTab = m_pWebManager->GetHudWebTab();
	g_pAnarchyManager->GetWebManager()->SelectWebTab(pHudWebTab);
	g_pAnarchyManager->GetInputManager()->ActivateInputMode(true);

	unsigned int uCount;
	std::string num;
	
	// And continue starting up
	uCount = m_pMetaverseManager->LoadAllLocalTypes();
	num = VarArgs("%u", uCount);
	pHudWebTab->AddHudLoadingMessage("progress", "", "Loading Types", "locallibrarytypes", "0", num, num);

	 //= m_pMetaverseManager->LoadAllLocalTypes();
	//std::string num = VarArgs("%u", uItemCount);
//	pHudWebTab->AddHudLoadingMessage("progress", "", "Loading Types", "locallibrarytypes", "0", num, num);

	uCount = m_pMetaverseManager->LoadAllLocalModels();
	num = VarArgs("%u", uCount);
	pHudWebTab->AddHudLoadingMessage("progress", "", "Loading Models", "locallibrarymodels", "0", num, num);
	
	//uItemCount = m_pMetaverseManager->LoadAllLocalApps();

	// load ALL local apps
	KeyValues* app = m_pMetaverseManager->LoadFirstLocalApp("MOD");
	if (app)
		pHudWebTab->AddHudLoadingMessage("progress", "", "Loading Apps", "locallibraryapps", "", "", "+", "loadNextLocalAppCallback");
	else
		this->OnLoadAllLocalAppsComplete();

}
*/

void C_AnarchyManager::OnLoadAllLocalAppsComplete()
{
	/*
	m_pMountManager = new C_MountManager();
	m_pMountManager->Init();
	m_pMountManager->LoadMountsFromKeyValues("mounts.txt");
	*/

	std::string path = "A:\\SteamLibrary\\steamapps\\common\\Anarchy Arcade\\aarcade\\";
	g_pFullFileSystem->AddSearchPath(path.c_str(), "MOD", PATH_ADD_TO_TAIL);
	g_pFullFileSystem->AddSearchPath(path.c_str(), "GAME", PATH_ADD_TO_TAIL);

	/* DISABLE MOUNTING OF LEGACY AARCADE CONTENT
	std::string pathDownload = "A:\\SteamLibrary\\steamapps\\common\\Anarchy Arcade\\aarcade\\download\\";	// for resolving cached images
	g_pFullFileSystem->AddSearchPath(path.c_str(), "MOD", PATH_ADD_TO_TAIL);
	g_pFullFileSystem->AddSearchPath(path.c_str(), "GAME", PATH_ADD_TO_TAIL);
	g_pAnarchyManager->GetMetaverseManager()->LoadFirstLocalItemLegacy(true, path, "", "");
	*/

	// INSTEAD, do the following:
	C_AwesomiumBrowserInstance* pHudBrowserInstance = m_pAwesomiumBrowserManager->FindAwesomiumBrowserInstance("hud");
	unsigned int uCount = g_pAnarchyManager->GetMetaverseManager()->LoadAllLocalItems();
	std::string num = VarArgs("%u", uCount);
	pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Loading Items", "locallibraryitems", "0", num, num);

	// AND THIS IS WHAT USUALLY GETS CALLED AFTER LOADFIRSTLOCALITEMLEGACY IS FINISHED:
	//g_pAnarchyManager->GetMetaverseManager()->LoadFirstLocalItemLegacy(true, "", "", "");



	//g_pAnarchyManager->GetMetaverseManager()->SetPreviousLocaLocalItemLegacyWorkshopIds("dummy");
	//g_pAnarchyManager->GetWorkshopManager()->OnMountWorkshopSucceed();
	this->OnMountAllWorkshopsComplete();	// the first time this is called initializes it all





	//C_AwesomiumBrowserInstance* pHudBrowserInstance = g_pAnarchyManager->GetAwesomiumBrowserManager()->FindAwesomiumBrowserInstance("hud");
	//pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Mounting Workshop Subscriptions", "mountworkshops", "", "0", "+", "mountNextWorkshopCallback");
}

bool C_AnarchyManager::AttemptSelectEntity()
{
	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return false;

	if (pPlayer->GetHealth() <= 0)
		return false;

	// fire a trace line
	trace_t tr;
	Vector forward;
	pPlayer->EyeVectors(&forward);
	UTIL_TraceLine(pPlayer->EyePosition(), pPlayer->EyePosition() + forward * MAX_COORD_RANGE, MASK_SOLID, pPlayer, COLLISION_GROUP_NONE, &tr);

	C_BaseEntity *pEntity = tr.m_pEnt;
	
	// only allow prop shortcuts
	C_PropShortcutEntity* pShortcut = null;
	if ( pEntity )
		pShortcut = dynamic_cast<C_PropShortcutEntity*>(pEntity);

	if (pShortcut && tr.fraction != 1.0 && tr.DidHitNonWorldEntity())
	{
		pEntity = tr.m_pEnt;

		if (m_pSelectedEntity && pEntity == m_pSelectedEntity)
		{
			//m_pInputManager->SetFullscreenMode(true);
			C_EmbeddedInstance* pEmbeddedInstance = m_pInputManager->GetEmbeddedInstance();
			m_pInputManager->ActivateInputMode(true, m_pInputManager->GetMainMenuMode(), pEmbeddedInstance);
		}
		else
			return SelectEntity(pEntity);
	}
	else
	{
		if (m_pSelectedEntity)
			return DeselectEntity(m_pSelectedEntity);
		else
			return false;
	}

	return false;
}

// from http://www.zedwood.com/article/cpp-urlencode-function
#include <iostream>
#include <sstream>
std::string C_AnarchyManager::encodeURIComponent(const std::string &s)
{
	static const char lookup[] = "0123456789abcdef";
	std::stringstream e;
	for (int i = 0, ix = s.length(); i<ix; i++)
	{
		const char& c = s[i];
		if ((48 <= c && c <= 57) ||//0-9
			(65 <= c && c <= 90) ||//abc...xyz
			(97 <= c && c <= 122) || //ABC...XYZ
			(c == '-' || c == '_' || c == '.' || c == '~')
			)
		{
			e << c;
		}
		else
		{
			e << '%';
			e << lookup[(c & 0xF0) >> 4];
			e << lookup[(c & 0x0F)];
		}
	}
	return e.str();
}

bool C_AnarchyManager::SelectEntity(C_BaseEntity* pEntity)
{
//	DevMsg("DISABLED FOR TESTING!\n");
//	return true;
//	/*
	if (m_pSelectedEntity)
		DeselectEntity(m_pSelectedEntity);

//	m_pWebManager->GetHudWebTab()->SetUrl("asset://ui/blank.html");

	m_pSelectedEntity = pEntity;
	AddGlowEffect(pEntity);

	

		//pMaterials[x]->ColorModulate(255, 0, 0);
	//pMaterials[x]->GetPreviewImage

	std::string itemId;
	std::string tabTitle;
	std::string uri;
	KeyValues* item;
	KeyValues* active;
	//C_PropShortcutEntity* pShortcut;
	//C_WebTab* pWebTab;
	//C_WebTab* pSelectedWebTab;
	C_EmbeddedInstance* pEmbeddedInstance;
	C_EmbeddedInstance* pSelectedEmbeddedInstance;

	C_PropShortcutEntity* pShortcut = dynamic_cast<C_PropShortcutEntity*>(pEntity);
	std::vector<C_EmbeddedInstance*> embeddedInstances;
	pShortcut->GetEmbeddedInstances(embeddedInstances);

	unsigned int i;
	unsigned int size = embeddedInstances.size();
	for (i = 0; i < size; i++)
	{
		pEmbeddedInstance = embeddedInstances[i];
		if (!pEmbeddedInstance)
			continue;

		if (pEmbeddedInstance->GetId() == "hud")
			continue;

		bool bImagesAndHandled = false;
		if (pEmbeddedInstance->GetId() == "images")
		{
			pShortcut = dynamic_cast<C_PropShortcutEntity*>(m_pSelectedEntity);
			if (pShortcut)
			{
				tabTitle = "auto" + pShortcut->GetItemId();
				pEmbeddedInstance = m_pCanvasManager->FindEmbeddedInstance(tabTitle);// this->GetWebManager()->FindWebTab(tabTitle);
				if (!pEmbeddedInstance)
				{
					itemId = pShortcut->GetItemId();
					item = m_pMetaverseManager->GetLibraryItem(itemId);
					if (item)
					{
						active = item->FindKey("current");
						if (!active)
							active = item->FindKey("local", true);

						/*
						std::string uri = "asset://ui/autoInspectItem.html?id=" + encodeURIComponent(itemId) + "&screen=" + encodeURIComponent(active->GetString("screen")) + "&marquee=" + encodeURIComponent(active->GetString("marquee")) + "&preview=" + encodeURIComponent(active->GetString("preview")) + "&reference=" + encodeURIComponent(active->GetString("reference")) + "&file=" + encodeURIComponent(active->GetString("file"));
						WebURL url = WebURL(WSLit(uri.c_str()));
						*/

						//std::string dumbUrl = "http://smarcade.net/dlcv2/view_youtube.php?id=";
						//std::string dumbUrl = active->GetString("file");

						// If this is a video file, play it in libretro instead of the browser
						std::string exts = "::mpg::mpeg::avi::mp4::mkv::";
						std::string fileExt = active->GetString("file");
						
						size_t found = fileExt.find_last_of(".");
						if (found != std::string::npos)
							fileExt = fileExt.substr(found + 1);

						found = exts.find("::" + fileExt + "::");
						if (found != std::string::npos && g_pFullFileSystem->FileExists(active->GetString("file")))
						{
							C_LibretroInstance* pLibretroInstance = m_pLibretroManager->CreateLibretroInstance();
							pLibretroInstance->Init(tabTitle);
							pLibretroInstance->LoadCore();
							pLibretroInstance->SetGame(active->GetString("file"));
							pEmbeddedInstance = pLibretroInstance;
						}
						else
						{
							std::string uri = "file://";
							uri += engine->GetGameDirectory();
							uri += "/resource/ui/html/autoInspectItem.html?id=" + encodeURIComponent(itemId) + "&screen=" + encodeURIComponent(active->GetString("screen")) + "&marquee=" + encodeURIComponent(active->GetString("marquee")) + "&preview=" + encodeURIComponent(active->GetString("preview")) + "&reference=" + encodeURIComponent(active->GetString("reference")) + "&file=" + encodeURIComponent(active->GetString("file"));

							DevMsg("Test URI is: %s\n", uri.c_str());	// FIXME: Might want to make the slashes in the game path go foward.  Also, need to allow HTTP redirection (302).

							C_SteamBrowserInstance* pSteamBrowserInstance = m_pSteamBrowserManager->CreateSteamBrowserInstance();
							pSteamBrowserInstance->Init(tabTitle, uri);
							pEmbeddedInstance = pSteamBrowserInstance;
						}
					}
					else
					{
						// the item specified by the shortcut was not found
						// by doing NOTHING, it lets you select the object but not bring up any menus on it
					}
				}
			}
		}
		else
		{
			pSelectedEmbeddedInstance = m_pInputManager->GetEmbeddedInstance();// m_pWebManager->GetSelectedWebTab();
			if (pSelectedEmbeddedInstance)
			{
				pSelectedEmbeddedInstance->Deselect();
				m_pInputManager->SetEmbeddedInstance(null);
				//m_pWebManager->DeselectWebTab(pSelectedEmbeddedInstance);
			}
		}

		if (pEmbeddedInstance)
		{
			pSelectedEmbeddedInstance = m_pInputManager->GetEmbeddedInstance();
			if (pSelectedEmbeddedInstance && pSelectedEmbeddedInstance != pEmbeddedInstance)
			{
				//m_pWebManager->DeselectWebTab(pSelectedEmbeddedInstance);
				pSelectedEmbeddedInstance->Deselect();
				m_pInputManager->SetEmbeddedInstance(null);

				//m_pWebManager->SelectWebTab(pWebTab);
				pEmbeddedInstance->Select();
				m_pInputManager->SetEmbeddedInstance(pEmbeddedInstance);
			}
			else if (!pSelectedEmbeddedInstance)
			{
				//if (pEmbeddedInstance)
				//{
				pEmbeddedInstance->Select();
				m_pInputManager->SetEmbeddedInstance(pEmbeddedInstance);
				//}



			}
		}
		else
			DevMsg("ERROR: No embedded instance!!\n");
		//m_pWebManager->SelectWebTab(pWebTab);

		//break;
	}

	return true;
	//*/
}

bool C_AnarchyManager::DeselectEntity(C_BaseEntity* pEntity, std::string nextUrl, bool bCloseInstance)
{
	C_EmbeddedInstance* pEmbeddedInstance = m_pInputManager->GetEmbeddedInstance();
	C_AwesomiumBrowserInstance* pHudBrowserInstance = m_pAwesomiumBrowserManager->FindAwesomiumBrowserInstance("hud");
	//C_WebTab* pWebTab = m_pWebManager->GetSelectedWebTab();
	if (pEmbeddedInstance)
	{	
		pEmbeddedInstance->Deselect();
		//pEmbeddedInstance->Blur();
		m_pInputManager->SetEmbeddedInstance(null);
		//m_pWebManager->DeselectWebTab(pWebTab);

		if (nextUrl != "")
			pHudBrowserInstance->SetUrl(nextUrl);
		else
			pHudBrowserInstance->SetUrl("asset://ui/blank.html");

		// ALWAYS close the selected web tab when de-selecting entities. (this has to be accounted for or changed when the continous play button gets re-enabled!)
		if (bCloseInstance && pEmbeddedInstance != pHudBrowserInstance)
			pEmbeddedInstance->Close();
	}

	RemoveGlowEffect(m_pSelectedEntity);
	m_pSelectedEntity = null;
	return true;
}

void C_AnarchyManager::AddGlowEffect(C_BaseEntity* pEntity)
{
	engine->ServerCmd(VarArgs("addgloweffect %i", pEntity->entindex()), false);
}

void C_AnarchyManager::RemoveGlowEffect(C_BaseEntity* pEntity)
{
	engine->ServerCmd(VarArgs("removegloweffect %i", pEntity->entindex()), false);
}

void C_AnarchyManager::ShowFileBrowseMenu()
{
	BrowseSlate->Create(enginevgui->GetPanel(PANEL_ROOT));
}

void C_AnarchyManager::OnWorkshopManagerReady()
{
//	DevMsg("DISABLED FOR TESTING!\n");
//	return;
	///*

	C_AwesomiumBrowserInstance* pHudBrowserInstance = m_pAwesomiumBrowserManager->FindAwesomiumBrowserInstance("hud");
	// mount ALL workshops
	pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Skipping Legacy Workshop Subscriptions", "skiplegacyworkshops", "", "", "0");
	pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Loading Workshop Models", "workshoplibrarymodels", "", "", "0");
	pHudBrowserInstance->AddHudLoadingMessage("progress", "", "Loading Workshop Items", "workshoplibraryitems", "", "", "0");

	m_pWorkshopManager->MountFirstWorkshop();
	//*/
}

void C_AnarchyManager::OnMountAllWorkshopsComplete()
{
	if (!m_pMountManager)	// it is our first time here
	{
		std::string path = "A:\\SteamLibrary\\steamapps\\common\\Anarchy Arcade\\aarcade\\";

		// detect any .set files in the legacy folder too
		std::string file;
		KeyValues* kv = new KeyValues("instance");
		FileFindHandle_t findHandle;
		//DevMsg("Tester folder: %smaps\\*.set", path);
		const char *pFilename = g_pFullFileSystem->FindFirstEx(VarArgs("%smaps\\*.set", path), "", &findHandle);
		while (pFilename != NULL)
		{
			if (g_pFullFileSystem->FindIsDirectory(findHandle))
			{
				pFilename = g_pFullFileSystem->FindNext(findHandle);
				continue;
			}

			file = std::string(path) + "maps\\" + std::string(pFilename);

			// FIXME: build an ACTUAL generation 3 instance key values here, and save it out!!
			if (kv->LoadFromFile(g_pFullFileSystem, file.c_str()))
			{
				if (kv->FindKey("map") && kv->FindKey("objects", true)->GetFirstSubKey())
				{
					//	DevMsg("Map ID here is: %s\n", kv->GetString("map"));
					// FIXME: instance_t's should have mapId's, not MapNames.  The "mapName" should be considered the title.  The issue is that maps usually haven't been detected by this point, so assigning a mapID based on the legacy map name is complex.
					// For now, mapId's will be resolved upon map detection if mapID's equal a detected map's filename.

					std::string title = kv->GetString("title");
					if (title == "")
					{
						//title = "Unnamed";
						title = file;
						size_t found = title.find_last_of("/\\");
						if (found != std::string::npos)
							title = title.substr(found + 1);
					}

					g_pAnarchyManager->GetInstanceManager()->AddInstance(g_pAnarchyManager->GenerateUniqueId(), kv->GetString("map"), title, file, "", "");
					//g_pAnarchyManager->GetInstanceManager()->AddInstance(g_pAnarchyManager->GenerateLegacyHash(kv->GetString("map")), kv->GetString("map"), kv->GetString("map"), file, "", "");
				}
			}

			kv->Clear();
			pFilename = g_pFullFileSystem->FindNext(findHandle);
		}
		g_pFullFileSystem->FindClose(findHandle);

		m_pMountManager = new C_MountManager();
		m_pMountManager->Init();
		m_pMountManager->LoadMountsFromKeyValues("mounts.txt");

		m_pWorkshopManager = new C_WorkshopManager();
		m_pWorkshopManager->Init();
	}
	else
		this->GetMetaverseManager()->DetectAllMaps();
		//this->OnDetectAllMapsComplete();
}

void C_AnarchyManager::OnDetectAllMapsComplete()
{
	//DevMsg("DISABLED FOR TESTING!\n");
	//return;
	///*
	m_pLibretroManager = new C_LibretroManager();

	if (m_iState < 1)
	{
		m_iState = 1;

		C_AwesomiumBrowserInstance* pHudBrowserInstance = m_pAwesomiumBrowserManager->GetSelectedAwesomiumBrowserInstance();
		pHudBrowserInstance->SetUrl("asset://ui/welcome.html");
	}
	else
	{
		DevMsg("Done again!!\n");
	}
	//*/
}

void C_AnarchyManager::Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
{
	std::string safeStr = str;
	std::transform(safeStr.begin(), safeStr.end(), safeStr.begin(), ::tolower);

	// Skip delimiters at beginning.
	std::string::size_type lastPos = safeStr.find_first_not_of(delimiters, 0);

	// Find first "non-delimiter".
	std::string::size_type pos = safeStr.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// Skip delimiters.  Note the "not_of"
		lastPos = safeStr.find_first_not_of(delimiters, pos);

		// Find next "non-delimiter"
		pos = safeStr.find_first_of(delimiters, lastPos);
	}
}

/*
void C_AnarchyManager::ReleaseFileBrowseParams()
{
	if (m_pFileParams)
	{
		delete m_pFileParams;
		m_pFileParams = null;
	}
}
*/

void C_AnarchyManager::TestSQLite()
{
	int rc;
	char *error;

	// Open Database
	DevMsg("Opening MyDb.db ...\n");

	sqlite3 *db;
	rc = sqlite3_open("MyDb.db", &db);
	if (rc)
	{
		DevMsg("Error opening SQLite3 database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
		//return 1;
	}
	else
	{
		DevMsg("Opened MyDb.db.\n");
	}

	// Execute SQL
	DevMsg("Creating MyTable ...\n");
	const char *sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
	rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
	if (rc)
	{
		DevMsg("Error executing SQLite3 statement: %s\n", sqlite3_errmsg(db));
		sqlite3_free(error);
	}
	else
	{
		DevMsg("Created MyTable.\n");
	}

	// Execute SQL
	DevMsg("Inserting a value into MyTable ...\n");
	const char *sqlInsert = "INSERT INTO MyTable VALUES(NULL, 'A Value');";
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	if (rc)
	{
		DevMsg("Error executing SQLite3 statement: %s\n", sqlite3_errmsg(db));
		sqlite3_free(error);
	}
	else
	{
		DevMsg("Inserted a value into MyTable.\n");
	}

	// Display MyTable
	DevMsg("Retrieving values in MyTable ...\n");
	const char *sqlSelect = "SELECT * FROM MyTable;";
	char **results = NULL;
	int rows, columns;
	sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
	if (rc)
	{
		DevMsg("Error executing SQLite3 query: %s\n", sqlite3_errmsg(db));
		sqlite3_free(error);
	}
	else
	{
		// Display Table
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				DevMsg("%s\t", results[cellPosition]);
				//cout.width(12);
				//cout.setf(ios::left);
				//cout << results[cellPosition] << " ";
			}

			// End Line
			//cout << endl;
			DevMsg("\n");

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					//cout.width(12);
					//cout.setf(ios::left);
					DevMsg("~~~~~~~~~~~~");
				}
				DevMsg("\n");
			}
		}
	}
	sqlite3_free_table(results);

	// Close Database
	DevMsg("Closing MyDb.db ...\n");
	sqlite3_close(db);
	DevMsg("Closed MyDb.db\n");
}

/*
void C_AnarchyManager::LevelInitPreEntity()
{
	DevMsg("AnarchyManager: LevelInitPreEntity\n");
	//m_pWebViewManager = new C_WebViewManager;
//	m_pWebViewManager->Init();
}

void C_AnarchyManager::LevelShutdownPostEntity()
{
	DevMsg("AnarchyManager: LevelShutdownPostEntity\n");
	// FIXME: Deleting the webview manager prevents it from starting up again.
	// Need to only create/delete it ONCE during the lifetime of AArcade.
	//delete m_pWebViewManager;
}
*/
/*
const char* C_AnarchyManager::GenerateHash(const char* text)
{
	char input[AA_MAX_STRING];
	Q_strcpy(input, text);

	// Convert it to lowercase & change all slashes to back-slashes
	V_FixSlashes(input);
	for( int i = 0; input[i] != '\0'; i++ )
		input[i] = tolower(input[i]);

	char lower[256];
	unsigned m_crc = 0xffffffff;

	int inputLength = strlen(input);
	for (int i = 0; i < inputLength; i++)
	{
		lower[i] = tolower(input[i]);
	}

	for (int i = 0; i < inputLength; i++)
	{
		unsigned c = lower[i];
		m_crc ^= (c << 24);

		for (int j = 0; j < 8; j++)
		{
			const unsigned FLAG = 0x80000000;
			if ((m_crc & FLAG) == FLAG)
			{
				m_crc = (m_crc << 1) ^ 0x04C11DB7;
			}
			else
			{
				m_crc <<= 1;
			}
		}
	}

	return VarArgs("%08x", m_crc);
}
*/