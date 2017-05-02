#pragma once

#include <stdint.h>
#include <functional>

namespace coroutine
{
	enum ECoroutineState
	{
		eCS_NONE,
		eCS_DEAD,	// ����״̬
		eCS_READY,	// �ȴ�ִ�У�������û�б�ִ�й���
		eCS_RUNNING,// ִ��״̬
		eCS_SUSPEND,// ����״̬
	};

	void		init(uint32_t nStackSize);
	/**
	@brief: ����Э�̣�����Э����ں���
	��������ջ��С��0�Ļ����ù���ջ��������ö���ջ
	*/
	uint64_t	create(uint32_t nStackSize, const std::function<void(uint64_t)>& fn);
	/**
	@brief: �ر�ָ��Э��
	*/
	void		close(uint64_t nID);
	/**
	@brief: �ָ�ĳһ��Э��ִ�У����Դ�һ��������yield�������غ��ȡ�������Э�̵�һ��ִ�о�����ں����Ĳ���
	*/
	void		resume(uint64_t nID, uint64_t nContext);
	/**
	@brief: ����ǰִ�е�Э�̣���Э���´λ�ִ��ʱ�᷵��resume�����������Ĳ���
	*/
	uint64_t	yield();
	/**
	@brief: ��ȡЭ��״̬
	*/
	uint32_t	getState(uint64_t nID);
	/**
	@brief: ����Э������
	*/
	void		setLocalData(uint64_t nID, const char* szName, uint64_t nData);
	/**
	@brief: ��ȡЭ������
	*/
	bool		getLocalData(uint64_t nID, const char* szName, uint64_t& nData);
	/**
	@brief: ɾ��Э������
	*/
	void		delLocalData(uint64_t nID, const char* szName);
	/**
	@brief: ��ȡ��ǰִ��Э�̵�ID
	*/
	uint64_t	getCurrentID();
	/**
	@brief: ��ȡЭ������
	*/
	uint32_t	getCoroutineCount();
	/**
	@brief: ��ȡ�ܵĶ���ջ��С
	*/
	uint64_t	getTotalStackSize();
};