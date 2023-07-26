#include "pch.h"
#include "Box.h"

//
//   -----------------------------
//   |                           |
//   |                           |
//   |                           |
//   |            A	(this)		 |
//	 |				------------------------------
//	 |				|			 |  			 |
//	 |				|      C     |  			 |
//	 |				| (重複部分) |               |
//	 |				|            |               |
//   --------------------------------------------|
//				    |							 |
//				    |			   B(collision)  |
//  			    |							 |
//  			    |							 |
//   			    ------------------------------
//

bool Box::isHitting(const Box * collision)
{
	return
		isHittingTop(collision) ||
		isHittingBottom(collision) ||
		isHittingRight(collision) ||
		isHittingLeft(collision);
}

bool Box::isHittingTop(const Box* collision)
{
	// 「当たっているオブジェクトのxの大きさ」に対する「重なっている部分のxの大きさ」の割合
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// 「当たっているオブジェクトのｙの大きさ」に対する「重なっている部分のｙの大きさ」の割合
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);
	
	// そもそも当たってない
	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}

	// 重なっている部分が左右の判定部分に、はみ出している
	if (!(c_yRadio	<= m_heightRadio))
	{
		return false;
	}


	//「自オブジェクトのｙ座標」が「当たっているオブジェクトのｙ座標」より下にある
	if (this->GetTransform()->position.y < collision->GetTransform()->position.y)
	{
		return false;
	}

	// オブジェクトの上部に当たってる
 	return true;
}

bool Box::isHittingBottom(const Box* collision)
{
	// 「当たっているオブジェクトのxの大きさ」に対する「重なっている部分のxの大きさ」の割合
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// 「当たっているオブジェクトのｙの大きさ」に対する「重なっている部分のｙの大きさ」の割合
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);

	// そもそも当たってない
	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}

	// 重なっている部分が左右の判定部分に、はみ出している
	if (!(c_yRadio <= m_heightRadio))
	{
		return false;
	}

	//「自オブジェクトのｙ座標」が「当たっているオブジェクトのｙ座標」より同じか上にある
	if (this->GetTransform()->position.y >= collision->GetTransform()->position.y)
	{
		return false;
	}

	// オブジェクトの下部に当たってる
	return true;
}

bool Box::isHittingRight(const Box* collision)
{
	// 「当たっているオブジェクトのxの大きさ」に対する「重なっている部分のxの大きさ」の割合
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// 「当たっているオブジェクトのｙの大きさ」に対する「重なっている部分のｙの大きさ」の割合
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);

	// そもそも当たってない

	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}


	// 重なっている部分が左右の判定部分に、はみ出していない
	if (!(c_yRadio > m_heightRadio))
	{
		return false;
	}

	//「自オブジェクトのx座標」が「当たっているオブジェクトのx座標」より左にある
	if (this->GetTransform()->position.x < collision->GetTransform()->position.x)
	{
		return false;
	}

	
	// オブジェクトの右部に当たってる
	return true;
}

bool Box::isHittingLeft(const Box* collision)
{
	// 「当たっているオブジェクトのxの大きさ」に対する「重なっている部分のxの大きさ」の割合
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// 「当たっているオブジェクトのｙの大きさ」に対する「重なっている部分のｙの大きさ」の割合
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);

	// そもそも当たってない
	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}

	// 重なっている部分が左右の判定部分に、はみ出していない
	if (!(c_yRadio > m_heightRadio))
	{
		return false;
	}

	//「自オブジェクトのx座標」が「当たっているオブジェクトのx座標」より同じか右にある
	if (this->GetTransform()->position.x >= collision->GetTransform()->position.x)
	{
		return false;
	}

	// オブジェクトの左部に当たってる
	return true;
}
