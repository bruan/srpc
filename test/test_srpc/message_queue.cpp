#include "stdafx.h"
#include "message_queue.h"

#define _DEFAULT_MESSAGE_QUEUE 1024

CMessageQueue::CMessageQueue()
	: m_nQueueBegin(0)
	, m_nQueueEnd(0)
	, m_nQueueCapacity(_DEFAULT_MESSAGE_QUEUE)
{
	this->m_pMessageQueue = nullptr;
}

CMessageQueue::~CMessageQueue()
{
	delete []this->m_pMessageQueue;
}

bool CMessageQueue::init()
{
	this->m_pMessageQueue = new SMessagePacket[this->m_nQueueCapacity];

	return true;
}

bool CMessageQueue::addMessagePacket(const SMessagePacket& sMessagePacket)
{
	std::unique_lock<std::mutex> guard(this->m_lock);

	this->m_pMessageQueue[this->m_nQueueEnd] = sMessagePacket;
	if (++this->m_nQueueEnd >= this->m_nQueueCapacity)
		this->m_nQueueEnd = 0;

	if (this->m_nQueueBegin == this->m_nQueueEnd)
	{
		SMessagePacket* pNewMessageQueue = new SMessagePacket[this->m_nQueueCapacity * 2];
		for (int32_t i = 0; i < this->m_nQueueCapacity; ++i)
			pNewMessageQueue[i] = this->m_pMessageQueue[(this->m_nQueueBegin + i) % this->m_nQueueCapacity];
		this->m_nQueueBegin = 0;
		this->m_nQueueEnd = this->m_nQueueCapacity;
		this->m_nQueueCapacity *= 2;
		delete []this->m_pMessageQueue;
		this->m_pMessageQueue = pNewMessageQueue;
	}

// 	if (this->m_nQueueEnd - this->m_nQueueBegin == 1 || this->m_nQueueBegin - this->m_nQueueEnd == this->m_nQueueCapacity - 1)
// 		this->m_cond.notify_one();

	return true;
}

void CMessageQueue::getMessagePacket(std::vector<SMessagePacket>& vecMessagePacket)
{
	vecMessagePacket.clear();

	std::unique_lock<std::mutex> guard(this->m_lock);

// 	while (this->m_nQueueEnd - this->m_nQueueBegin == 0)
// 	{
// 		this->m_cond.wait(guard);
// 	}

	if (this->m_nQueueEnd - this->m_nQueueBegin == 0)
		return;

	while (this->m_nQueueEnd - this->m_nQueueBegin != 0)
	{
		const SMessagePacket& sMessagePacket = this->m_pMessageQueue[this->m_nQueueBegin];
		vecMessagePacket.push_back(sMessagePacket);

		if (++this->m_nQueueBegin >= this->m_nQueueCapacity)
			this->m_nQueueBegin = 0;
	}
}