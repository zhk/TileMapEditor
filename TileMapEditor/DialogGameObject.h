#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "GridTypes.h"

class ResourceGameObject;
class ResourceGameObjectGroup;

//游戏对象编辑器中，参数面板
class CDialogGameObject : public CDialog, public GridObject
{
	DECLARE_DYNAMIC(CDialogGameObject)

public:
	CDialogGameObject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogGameObject();

// Dialog Data
	enum { IDD = IDD_DIALOG_GO_EDITOR };

	//绘制正在编辑的游戏对象
	void			DrawEditingObject(CDC* pDC);

	//添加/编辑后被触发
	void			AfterSetData(bool bCopy);

	//是否点击中编辑对象判断
	bool			HitTest(CPoint ptPixel);

	//移动编辑对象
	void			MoveGameObject(CPoint ptOffset);

	//添加阻挡点
	bool			AddObstacle(CPoint ptPixel);

	//删除阻挡点
	bool			ClearObstacle(CPoint ptPixel);

protected:

	void			UpdateCenterInfo();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	bool						_bAdd;		//添加还是编辑现有的
	ResourceGameObject*			_pResGO;	//当前游戏对象
	ResourceGameObjectGroup*	_pGOGroup;	//当前游戏对象组


	CString				_strGOGroupName;	//组名
	CString				_strGOName;			//游戏对象名
	CSpinButtonCtrl		_spinTileCount;		//快捷编辑Tile数量
	CString				_strCenterOffset;	//绘制左上角相对图像中心偏移
	CString				_strResArtGroup;	//依赖的美术资源组名
	CComboBox			_comboArt;			//依赖的美术资源
	int					_iMode;				//鼠标模式
	CString				_strMapType;		//地图模式名称
	ObstacleListType	_obstacles;			//阻挡列表
	CPoint				_ptOffset;			//绘制左上角相对图像中心偏移
	CString				_strAIType;			//AI类型

	CPoint				_ptSelected;		//当前绘制的起点
	CSize				_szSelected;		//当前绘制区域大小

	afx_msg void OnBnClickedButtonGoOk();
	afx_msg void OnBnClickedButtonGoCancel();
	afx_msg void OnEnChangeEditGoTileCount();
	afx_msg void OnCbnSelchangeComboGoArtid();
	afx_msg void OnBnClickedRadioGoSetObstacle();
	afx_msg void OnBnClickedRadioGoClearObstacle();
	afx_msg void OnBnClickedRadioGoSelect();
};
