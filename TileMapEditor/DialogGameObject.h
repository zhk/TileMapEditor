#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "GridTypes.h"

class ResourceGameObject;
class ResourceGameObjectGroup;

//��Ϸ����༭���У��������
class CDialogGameObject : public CDialog, public GridObject
{
	DECLARE_DYNAMIC(CDialogGameObject)

public:
	CDialogGameObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogGameObject();

// Dialog Data
	enum { IDD = IDD_DIALOG_GO_EDITOR };

	//�������ڱ༭����Ϸ����
	void			DrawEditingObject(CDC* pDC);

	//���/�༭�󱻴���
	void			AfterSetData(bool bCopy);

	//�Ƿ����б༭�����ж�
	bool			HitTest(CPoint ptPixel);

	//�ƶ��༭����
	void			MoveGameObject(CPoint ptOffset);

	//����赲��
	bool			AddObstacle(CPoint ptPixel);

	//ɾ���赲��
	bool			ClearObstacle(CPoint ptPixel);

protected:

	void			UpdateCenterInfo();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	bool						_bAdd;		//��ӻ��Ǳ༭���е�
	ResourceGameObject*			_pResGO;	//��ǰ��Ϸ����
	ResourceGameObjectGroup*	_pGOGroup;	//��ǰ��Ϸ������


	CString				_strGOGroupName;	//����
	CString				_strGOName;			//��Ϸ������
	CSpinButtonCtrl		_spinTileCount;		//��ݱ༭Tile����
	CString				_strCenterOffset;	//�������Ͻ����ͼ������ƫ��
	CString				_strResArtGroup;	//������������Դ����
	CComboBox			_comboArt;			//������������Դ
	int					_iMode;				//���ģʽ
	CString				_strMapType;		//��ͼģʽ����
	ObstacleListType	_obstacles;			//�赲�б�
	CPoint				_ptOffset;			//�������Ͻ����ͼ������ƫ��
	CString				_strAIType;			//AI����

	CPoint				_ptSelected;		//��ǰ���Ƶ����
	CSize				_szSelected;		//��ǰ���������С

	afx_msg void OnBnClickedButtonGoOk();
	afx_msg void OnBnClickedButtonGoCancel();
	afx_msg void OnEnChangeEditGoTileCount();
	afx_msg void OnCbnSelchangeComboGoArtid();
	afx_msg void OnBnClickedRadioGoSetObstacle();
	afx_msg void OnBnClickedRadioGoClearObstacle();
	afx_msg void OnBnClickedRadioGoSelect();
};
