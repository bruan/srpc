#include <stdint.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

struct	SMessagePacket
{
	void*		pData;
	uint32_t	nDataSize;
};

class CMessageQueue
{
public:
	CMessageQueue();
	virtual ~CMessageQueue();

	bool					init();
	bool					addMessagePacket(const SMessagePacket& sMessagePacket);
	void					getMessagePacket(std::vector<SMessagePacket>& vecMessagePacket);

private:
	SMessagePacket*			m_pMessageQueue;
	int32_t					m_nQueueBegin;
	int32_t					m_nQueueEnd;
	int32_t					m_nQueueCapacity;

	std::mutex				m_lock;
	std::condition_variable	m_cond;
};