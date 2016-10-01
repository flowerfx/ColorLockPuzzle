#include "SoundManager.h"
#include "FileManager.h"
#include "DisplayView.h"
namespace Utility
{
	namespace Sound
	{
        std::mutex _mutex;
		SoundManager * SoundManager::p_Instance = NULL;

		SoundManager::SoundManager() 
#ifdef USE_AUDIO_ON_THREAD
			: m_criticalSection(0)
			, p_ThreadUpdatePushSound(0)
			, m_exit(0)
			, m_condition(0)
			, p_ThreadUpdatePushSoundTemp(0)
			, m_exit_temp(0)
			, m_condition_temp(0)
#endif
		{
			p_IsInitialized = false;
			p_currentIdsoundfx = 0;
			//
			p_currentMusicPath = "";
#ifdef USE_AUDIO_ON_THREAD
			p_list_id_sound_push_temp.clear();
			p_list_id_sound_push.clear();
#endif
		}
#ifdef USE_AUDIO_ON_THREAD
		void SoundManager::ReleaseSoundThread()
		{
			if (m_criticalSection)
			{
				RKCriticalSection_Destroy(&m_criticalSection);
			}

			bool bExit = false;
			if (m_exit)
			{
				bExit = RKInterlock_Query(m_exit) != 0;
			}

			if (!bExit)
			{
				// tell the work thread to exit
				RKInterlock_Lock(m_exit);

				// wake work thread
				if (m_condition)
				{
					RKThreadCondition_WakeAll(m_condition);
				}

				if (p_ThreadUpdatePushSound)
				{
					RKUtils::RKThread_WaitForExit(p_ThreadUpdatePushSound);
					RKUtils::RKThread_Destroy(&p_ThreadUpdatePushSound);
				}

				if (m_condition)
				{
					RKThreadCondition_Destroy(&m_condition);
				}

				if (!m_exit)
				{
					RKInterlock_Destroy(&m_exit);
				}
			}

			bExit = false;
			if (m_exit)
			{
				bExit = RKInterlock_Query(m_exit_temp) != 0;
			}

			if (!bExit)
			{
				// tell the work thread to exit
				RKInterlock_Lock(m_exit_temp);

				// wake work thread
				if (m_condition)
				{
					RKThreadCondition_WakeAll(m_condition_temp);
				}

				if (p_ThreadUpdatePushSoundTemp)
				{
					RKUtils::RKThread_WaitForExit(p_ThreadUpdatePushSoundTemp);
					RKUtils::RKThread_Destroy(&p_ThreadUpdatePushSoundTemp);
				}

				if (m_condition_temp)
				{
					RKThreadCondition_Destroy(&m_condition_temp);
				}

				if (!m_exit_temp)
				{
					RKInterlock_Destroy(&m_exit_temp);
				}
			}

		}
#endif
		SoundManager::~SoundManager()
		{
#ifdef USE_AUDIO_ON_THREAD
			p_list_id_sound_push_temp.clear();
			p_list_id_sound_push.clear();

			ReleaseSoundThread();
#endif
		}
		void SoundManager::InitSoundEngine()
		{
			p_IsInitialized = true;

			bool p_IsEnableSound = (File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_EN_SOUND) == "1") ? true : false;
			bool p_IsEnableMusic = (File::SaveMgr->GetDataSave()->GetcharByName(SAVE_STR_EN_MUSIC) == "1") ? true : false;

			if (p_IsEnableSound)
			{
				ResetVolume();
			}
			else
			{
				MuteVolume();
			}
			///
			if (p_IsEnableMusic)
			{
				ResetVolume(false);
			}
			else
			{
				MuteVolume(false);
			}

#ifdef USE_AUDIO_ON_THREAD
			p_IsOnUpdateThread = false;
			p_IsStartThread = false;

			m_criticalSection = RKCriticalSection_Create("push_mutex");

			m_condition = RKThreadCondition_Create("UpdateSound_Cond");
			m_condition_temp = RKThreadCondition_Create("UpdateSoundTemp_Cond");

			m_exit = RKInterlock_Create("UpdateSound_Exit");
			RKInterlock_Reset(m_exit);

			m_exit_temp = RKInterlock_Create("UpdatSound_Exit_temp");
			RKInterlock_Reset(m_exit_temp);
            
            p_ThreadUpdatePushSound = RKUtils::RKThread_Create("thread_play_sound", &SoundManager::ThreadUpdatePushSound, this, RKThreadPriority::RKThreadPriority_Lowest);
            RKThread_Start(p_ThreadUpdatePushSound);
            p_ThreadUpdatePushSoundTemp = RKUtils::RKThread_Create("thread_play_sound_temp", &SoundManager::ThreadUpdatePushToTemp, this, RKThreadPriority::RKThreadPriority_Lowest);
            RKThread_Start(p_ThreadUpdatePushSoundTemp);
#endif

		}
		//
		//for the system interrupt
		//
		void SoundManager::ResumeSoundEngine()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->resumeAllEffects();
			GetSimpleSound->resumeBackgroundMusic();
		}
		void SoundManager::PauseSoundEngine()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->pauseAllEffects();
			GetSimpleSound->pauseBackgroundMusic();
		}
		//
		void SoundManager::PlaySoundEffect(RKString ID_SOUND, float delay, int loop, bool imediately)
		{
			if (!p_IsInitialized)
				return;
			
			if (ID_SOUND == "")
				return;
            
			if (!imediately)
			{
#ifdef USE_AUDIO_ON_THREAD
                _mutex.lock();
				p_list_id_sound_push_temp.push_back(
					new SoundDes(ID_SOUND, delay, loop));
                _mutex.unlock();
				OnStartThreadPushSound();
				return;
#endif

			}

			RKString full_path = XMLMgr->GetSoundDecByName(ID_SOUND)->path;
			full_path = File::ResolveNamePath(full_path);

			p_currentIdsoundfx = GetSimpleSound->playEffect(full_path.GetString() , false);
		}
		void SoundManager::PlayMusicEffect(RKString ID_MUSIC, bool loop, bool use_in_thread)
		{
			//p_currentMusicPath = namePath;
			if (!p_IsInitialized)
				return;
			if (ID_MUSIC == "")
				return;
            
//			if (use_in_thread)
//			{
//#ifdef USE_AUDIO_ON_THREAD
//				p_list_id_sound_push_temp.insert(p_list_id_sound_push_temp.begin() + p_list_id_sound_push_temp.size(),
//					new SoundDes(ID_MUSIC, 0.1f, loop ? 1 : 0));
//				OnStartThreadPushSound();
//				return;
//#endif
//			}

			RKString full_path = XMLMgr->GetSoundDecByName(ID_MUSIC)->path;
			full_path = File::ResolveNamePath(full_path);
            if(IsMusicPlaying())
            {
                StopTheCurrentSound();
            }
			GetSimpleSound->playBackgroundMusic(full_path.GetString(), loop);
		}

		void SoundManager::PlayCurrentMusic()
		{
			if (!p_IsInitialized)
				return;
			if (p_currentMusicPath == "")
				return;
			GetSimpleSound->rewindBackgroundMusic();
		}
		//
		void SoundManager::PauseTheCurrentSound()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->pauseEffect(p_currentIdsoundfx);
		}
		void SoundManager::StopTheCurrentSound()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->stopEffect(p_currentIdsoundfx);
		}
		void SoundManager::ResumeTheCurrentSound()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->resumeEffect(p_currentIdsoundfx);
		}

		void SoundManager::PauseTheCurrentMusic()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->pauseBackgroundMusic();
		}

		void SoundManager::StopTheCurrentMusic()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->stopBackgroundMusic(true);
		}

		void SoundManager::ResumeTheCurrentMusic()
		{
			if (!p_IsInitialized)
				return;
			GetSimpleSound->resumeBackgroundMusic();
		}

		bool SoundManager::IsMusicPlaying()
		{
			if (!p_IsInitialized)
				return false;
			return GetSimpleSound->isBackgroundMusicPlaying();
		}

		void SoundManager::MuteVolume(bool sound)
		{
			if (!p_IsInitialized)
				return;
			if (!sound)
			{
				GetSimpleSound->setBackgroundMusicVolume(0.f);
			}
			else
			{
				GetSimpleSound->setEffectsVolume(0.f);
			}
		}
		void SoundManager::ResetVolume(bool sound)
		{
			if (!p_IsInitialized)
				return;
			if (!sound)
			{
				GetSimpleSound->setBackgroundMusicVolume(0.7f);
			}
			else
			{
				GetSimpleSound->setEffectsVolume(0.5f);
			}
		}


#ifdef USE_AUDIO_ON_THREAD
		uint32 SoundManager::ThreadUpdatePushSound(void* pThreadData)
		{
			SoundManager * m_This = (SoundManager*)pThreadData;
			if (m_This)
			{
				m_This->ThreadPushSound();
			}

			return 0;
		}

		uint32 SoundManager::ThreadUpdatePushToTemp(void* pThreadData)
		{
			SoundManager * m_This = (SoundManager*)pThreadData;
			if (m_This)
			{
				m_This->ThreadPushSoundTemp();
			}

			return 0;
		}

		void SoundManager::ThreadPushSound()
		{

			// keep the thread running until we want to stop it
			bool bExit = RKInterlock_Query(m_exit) != 0;
			while (!bExit)
			{

				// wait for work to be added
				RKCriticalSection_Enter(m_criticalSection);
				bExit = RKInterlock_Query(m_exit) != 0;

				while (!p_IsOnUpdateThread && !bExit)
				{
					RKThreadCondition_Sleep(m_condition, m_criticalSection, 0xFFFFFFF); //sleep infinity and wait wake up
					bExit = RKInterlock_Query(m_exit) != 0;
				}

				if (bExit)
				{
					RKCriticalSection_Leave(m_criticalSection);
					break;
				}

				for (int i = 0; i < p_list_id_sound_push.size(); i++)
				{
					auto detail_sound_data = XMLMgr->GetSoundDecByName(p_list_id_sound_push[i]->ID_SOUND);
					if (detail_sound_data && detail_sound_data->type_sound == "sound")
					{
						RKString full_path = detail_sound_data->path;
						full_path = File::ResolveNamePath(full_path);

						int loop = p_list_id_sound_push[i]->loop > 0 ? p_list_id_sound_push[i]->loop : detail_sound_data->loop;
						float delay_first = p_list_id_sound_push[i]->delayTime > 0 ? p_list_id_sound_push[i]->delayTime : detail_sound_data->delay_first;

						for (int j = 0; j < loop; j++)
						{
							RKUtils::RKThread_Sleep(0, delay_first * 1000);

							p_currentIdsoundfx = GetSimpleSound->playEffect(full_path.GetString(), false);
						}
					}
					else if (detail_sound_data && detail_sound_data->type_sound == "music")
					{
						RKString full_path = detail_sound_data->path;
						full_path = File::ResolveNamePath(full_path);

						int loop = p_list_id_sound_push[i]->loop > 0 ? p_list_id_sound_push[i]->loop : detail_sound_data->loop;
						float delay_first = p_list_id_sound_push[i]->delayTime > 0 ? p_list_id_sound_push[i]->delayTime : detail_sound_data->delay_first;
			
						if (IsMusicPlaying())
						{
							StopTheCurrentSound();
						}

						RKUtils::RKThread_Sleep(0, delay_first * 1000);
						GetSimpleSound->playBackgroundMusic(full_path.GetString(), loop > 0 ? true : false) ;
					}
				}
				p_list_id_sound_push.clear();

				p_IsOnUpdateThread = false;

				RKCriticalSection_Leave(m_criticalSection);
			}
			RKInterlock_Reset(m_exit);
		}
		void SoundManager::ThreadPushSoundTemp()
		{
			bool bExit = RKInterlock_Query(m_exit_temp) != 0;
			while (!bExit)
			{

				// wait for work to be added
				RKCriticalSection_Enter(m_criticalSection);
				bExit = RKInterlock_Query(m_exit_temp) != 0;

				while (!p_IsStartThread && !bExit)
				{
					RKThreadCondition_Sleep(m_condition_temp, m_criticalSection, 0xFFFFFFF); //sleep infinity and wait wake up
					bExit = RKInterlock_Query(m_exit_temp) != 0;
				}

				if (bExit)
				{
					RKCriticalSection_Leave(m_criticalSection);
					break;
				}

				OnPushFromTempToMain();

				p_IsStartThread = false;

				RKUtils::RKCriticalSection_Leave(m_criticalSection);
			}
			RKInterlock_Reset(m_exit_temp);
		}

		void SoundManager::OnPushFromTempToMain()
		{
			for (int i = 0; i < p_list_id_sound_push_temp.size(); i++)
			{
				p_list_id_sound_push.insert(p_list_id_sound_push.begin() + p_list_id_sound_push.size(), p_list_id_sound_push_temp.at(i));
			}
			p_list_id_sound_push_temp.clear();
			p_IsOnUpdateThread = true;
			RKThreadCondition_WakeAll(m_condition);
		}
#endif
	}
}