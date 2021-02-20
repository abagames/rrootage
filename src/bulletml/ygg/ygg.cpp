/******************************************************************************
	���E�� -yggdrasil-
		���E�����W���[���\�[�X�t�@�C��
											Coded by Wraith	in July 14, 2002.
******************************************************************************/

//�@Tab�����S�����ɐݒ肵�ĕ\�������Ă��������B

///////////////////////////////////////////////////////////////////////////////
//
//	�� ygg.cpp
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.cpp
//
//	�� �֘A�t�@�C��
//		�{���W���[���̃w�b�_�t�@�C��
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.h
//		�{���W���[���̃`���[�g���A���\�[�X�t�@�C��
//		http://tricklib.com/cxx/ex/yggdrasil/ygg_test.cpp
//		�S�t�@�C���p�b�N
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.lzh
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.zip
//
//	�� ���t�@�����X�E�T�|�[�g�y�[�W
//		http://tricklib.com/cxx/ex/yggdrasil/
//
//	�� ���C�Z���X���
//		http://tricklib.com/license.htm
//

#include "ygg.h"

#include <iterator>
#include <map>

#ifdef __USING_STRINGSTREAM___
#	include <sstream>
#else
#	include <cstdio>
#endif

/******************************************************************************
	��������                      TrickPalace                       ��������
	��������              http://www.trickpalace.net/               ��������
******************************************************************************/

#define ARRAY_SIZE(X)  		(sizeof(X)/sizeof(X[0]))
#define ARRAY_END(X)   		(X +ARRAY_SIZE(X))
#define AD_LIBTIUM(XXX)		if (!(XXX)) ; else (*(XXX))
#define PROMOTION(XXX) 		XXX = XXX

#define init_stack \
char init_stack_dummy[1]; \
init_stack_dummy[0] = 0;

#if defined(_MSC_VER)
#	define tricklib_alloca _alloca
#else
#	define tricklib_alloca	alloca
#endif
#define get_stack(type, volume) ((type*)tricklib_alloca(volume*sizeof(type)))


//#define AD_LIB(XXX)					if (!(XXX)) ; else
//#define AD_LIBTIUM_WHILE(XXX)			while(XXX) (XXX)
//#define AD_LIB_WHILE_PROMOTION(XXX)	while(XXX) XXX = XXX

/******************************************************************************
	��������                       cppll ML                         ��������
	��������           http://www.trickpalace.net/cppll/            ��������
******************************************************************************/

namespace yggdrasil {

namespace ygg_utility {

ygg_string ygg_utility::make_indent(int indent) {
	if (0 <= indent) {
		return make_indent(indent -1) + ygg_term::indent;			//	return
	} else {
		return ygg_term::empty;										//	return
	}
}
ygg_string ygg_utility::encode_xml(const ygg_string &X) {
	ygg_string X_text = X;
	ygg_string_replace(X_text, "&", "&amp;");
	ygg_string_replace(X_text, "<", "&lt;");
	ygg_string_replace(X_text, ">", "&gt;");
	ygg_string_replace(X_text, "=", "&#61;");
//	ygg_string_replace(X_text, "\t", "&#9;");
	return X_text;													//	return
}
ygg_string ygg_utility::encode_attribute(const ygg_string &X) {
	ygg_string X_text = X;
	ygg_string_replace(X_text, "&", "&amp;");
	ygg_string_replace(X_text, "<", "&lt;");
	ygg_string_replace(X_text, ">", "&gt;");
	ygg_string_replace(X_text, "\"", "&quot;");
	ygg_string_replace(X_text, "\'", "&apos;");	//	&#39;
	ygg_string_replace(X_text, "=", "&#61;");
	ygg_string_replace(X_text, "\t", "&#9;");
	return X_text;													//	return
}
ygg_string ygg_utility::encode_sox(const ygg_string &X) {
	ygg_string X_text = X;
	ygg_string_replace(X_text, "&", "&amp;");
	ygg_string_replace(X_text, "<", "&lt;");
	ygg_string_replace(X_text, ">", "&gt;");
	ygg_string_replace(X_text, "=", "&#61;");
	ygg_string_replace(X_text, "\t", "&#9;");
	return X_text;													//	return
}
ygg_string ygg_utility::encode_sox(int indent, const ygg_string &X) {
	return ygg_utility::encode_sox(make_indent(indent), X);			//	return
}
ygg_string ygg_utility::encode_sox(const ygg_string &indent, const ygg_string &X) {
	ygg_string X_text;
	ygg_string buffer = X;
	ygg_string::size_type p;
	const int return_code_length = ygg_term::return_code.length();
	while(ygg_string::npos != (p = buffer.find(ygg_term::return_code))) {
		X_text += indent +encode_sox(buffer.substr(0, p)) +ygg_term::return_code;
		buffer = buffer.substr(p +return_code_length);
	}
	X_text += indent +encode_sox(buffer);
	return X_text;													//	return
//	return indent +encode_sox(X);
}
#if defined(__BORLANDC__)
#	pragma warn -8026
#	pragma warn -8027
#endif
ygg_string decode_xml(const ygg_string &X) {

//#define __SIMPLE_XML_CODING__
#ifdef __SIMPLE_XML_CODING__

	ygg_string X_text = X;
	ygg_string_replace(X_text, "&lt;", "<"),
	ygg_string_replace(X_text, "&gt;", ">"),
	ygg_string_replace(X_text, "&quot;", "\""),
	ygg_string_replace(X_text, "&apos;", "\'"),
	ygg_string_replace(X_text, "&#39;", "\'"),
	ygg_string_replace(X_text, "&#61;", "="),
	ygg_string_replace(X_text, "&#9;", "\t"),
	ygg_string_replace(X_text, "&amp;", "&");
	return X_text;													//	return

#else

	class decodex {
		public:
		ygg_string replace;
		decodex(const ygg_string source) {
			if (0 == source.find("&#")) {
				unsigned int code;
				if (0 == source.find("&#x")) {
					code = strtol(source.substr(3).c_str(), NULL, 16);
				} else {
					code = strtol(source.substr(2).c_str(), NULL, 10);
				}
#ifdef	__USING_UTF8__
				char utf8[8] = {0}; // ���w�肳��ĂȂ��v�f���[���ŏ����������B
				if (code < 0x80) {
					utf8[0] = (unsigned char)code;
				} else if (code < 0x800) {
					utf8[0] = 0xC0 | (code >> 6);
					utf8[1] = 0x80 | 0x3F & code;
				} else {
//				} else if (code < 0x10000) {
					utf8[0] = 0xE0 | (code >> 12);
					utf8[1] = 0x80 | 0x3F & (code >> 6);
					utf8[2] = 0x80 | 0x3F & code;
//				} else if (code < 0x200000) {
//					utf8[0] = 0xF0 | (code >> 18);
//					utf8[1] = 0x80 | 0x3F & (code >> 12);
//					utf8[2] = 0x80 | 0x3F & (code >> 6);
//					utf8[3] = 0x80 | 0x3F & code;
//				} else if (code < 0x400000) {
//					utf8[0] = 0xF8 | (code >> 24);
//					utf8[1] = 0x80 | 0x3F & (code >> 18);
//					utf8[2] = 0x80 | 0x3F & (code >> 12);
//					utf8[3] = 0x80 | 0x3F & (code >> 6);
//					utf8[4] = 0x80 | 0x3F & code;
//				} else {
//					utf8[0] = 0xFC | (code >> 30);
//					utf8[1] = 0x80 | 0x3F & (code >> 24);
//					utf8[2] = 0x80 | 0x3F & (code >> 18);
//					utf8[3] = 0x80 | 0x3F & (code >> 12);
//					utf8[4] = 0x80 | 0x3F & (code >> 6);
//					utf8[5] = 0x80 | 0x3F & code;
				}
				replace = utf8;
#else	//	__USING_UTF8__

				replace = "";

#	ifdef	__ACCEPT_NULLCODE__
				do {
					replace += (char)(code % 0x100);
					code /= 0x100;
				} while(code);
#	else	//	__ACCEPT_NULLCODE__
				while(code) {
					replace += (char)(code % 0x100);
					code /= 0x100;
				}
#	endif	//	__ACCEPT_NULLCODE__

#endif	//	__USING_UTF8__
			} else {
				assert(false);
				replace = source;
			}
			assert(0 < source.find(";"));
		}
		operator const char * () {
			return replace.c_str();									//	return
		}
	};

	ygg_string X_text = X;
	ygg_string X_search_begin = "&";
	ygg_string X_search_end = ";";
	ygg_string X_replace;
	ygg_string::size_type inc;
	ygg_string::size_type p_end;

	for(ygg_string::size_type p = 0;
		ygg_string::npos != (p = X_text.find(X_search_begin, p));
		p += inc) {

		if (ygg_string::npos == (p_end = X_text.find(X_search_end, p))) {
			break;													//	break
		}

		p == X_text.find("&lt;", p)		&&	(X_replace = "<",	true)	||
		p == X_text.find("&gt;", p)		&&	(X_replace = ">",	true)	||
		p == X_text.find("&quot;", p)	&&	(X_replace = "\"",	true)	||
		p == X_text.find("&apos;", p)	&&	(X_replace = "\'",	true)	||
		p == X_text.find("&amp;", p)	&&	(X_replace = "&",	true)	||
		p == X_text.find("&#", p)		&&	(X_replace =
							decodex(X_text.substr(p, p_end)),	true)	||
										(X_replace = X_text.substr(p, p_end +1),	true); // �������A�o�O���ĂȂ��H

		X_text.replace(p, (p_end -p) +1, X_replace);
		inc = X_replace.length();
	}

	return X_text;													//	break

#endif
}
#if defined(__BORLANDC__)
#	pragma warn .8026
#	pragma warn .8027
#endif


ygg_string ygg_utility::decode_attribute(const ygg_string &X) {
	return ygg_utility::decode_xml(X);								//	return
}
ygg_string ygg_utility::decode_sox(int , const ygg_string &X) {
//	indent;
	return decode_xml(X);											//	return
}

ygg_node & ygg_utility::format_tree(ygg_node &node, unsigned int max_row_length) {
	ygg_string type;
	bool is_text_node = ygg_node_type::root == node.get_type();
	bool is_simple_element = true;
	ygg_node list = ygg_list::create();
	for(ygg_iterator i = node[ygg_term::node].begin(); i.is_not_end(); ++i) {
		type = i->get_type();
		if (ygg_node_type::attribute != type) {
			is_simple_element = false;
			if (ygg_node_type::text == type) {
				is_text_node = true;
			} else {
				if (!is_text_node) {
					list.adopt_node(ygg_text::create().set_value(" "));
				}
				if (ygg_node_type::element == type && max_row_length < i->get_xml().length()) {
					format_tree(*i);
				}
				is_text_node = false;
			}
		}
		list.adopt_node(*i);
	}
	if (!is_simple_element && !is_text_node) {
		list.adopt_node(ygg_text::create().set_value(" "));
	}
	node[ygg_term::node].vanish();
	node.adopt_node(list);
	return node;
}

ygg_string ygg_utility::xml_trim(const ygg_string &X) {
	ygg_string X_text = X;

	for(ygg_string::size_type p = 0, p_end;
		ygg_string::npos != (p = X_text.find_first_of(ygg_term::white_space, p));
		p += 1) {
		p_end = X_text.find_first_not_of(ygg_term::white_space, p);
		X_text.replace(p, (ygg_string::npos != p_end) ? p_end -p: ygg_string::npos, " ");
	}

	return X_text;
}
ygg_string ygg_utility::both_trim(const ygg_string &X) {
	ygg_string X_text = X;
	ygg_string::size_type start_pos = X_text.find_first_not_of(ygg_term::white_space);
	if (ygg_string::npos == start_pos) {
		return "";
	}
	X_text = X_text.substr(start_pos);
	ygg_string::size_type end_pos = X_text.find_last_not_of(ygg_term::white_space);
	if (ygg_string::npos != end_pos && end_pos +1 < X_text.length()) {
		X_text = X_text.substr(0, end_pos +1);
	}
	return X_text;
}

ygg_error_code ygg_utility::check_name(const ygg_string &X) {
	using namespace ygg_term;
	using namespace ygg_error_term;

	//	�ꕶ���ڂ̃`�F�b�N
	if (ygg_string::npos != ygg_invalid_name_chars_a.find(X.substr(0, 1))) {
		return ygg_invalid_name_a;
	}

	//	�񕶎��ڈȍ~�̃`�F�b�N
#if	defined(__USING_UNKNOWN__)
	if (base_encoding::sjis == get_base_encoding()) {
#endif
#if	defined(__USING_UNKNOWN__) || defined(__USING_SJIS__)
		for(ygg_string::const_iterator i = X.begin(); i != X.end(); ++i) {
			if (0x81 <= (unsigned char)*i && (unsigned char)*i <= 0xFC) {
			//	�Q�o�C�g�R�[�h�̂P�o�C�g��...
				++i;
				if (i == X.end() || 0x40 <= (unsigned char)*i && (unsigned char)*i <= 0xFC) {
				//	�Q�o�C�g�R�[�h�̂Q�o�C�g�ڂ������A�������͂Q�o�C�g�ڂ̃R�[�h����蓾��͈͊O�̒l...
					return ygg_broken_char;
				}
			} else {
				if (ygg_string::npos != ygg_invalid_name_chars_b.find(*i)) {
					return ygg_invalid_name_b;
				}
			}
		}
#endif
#if	defined(__USING_UNKNOWN__)
	} else {
#endif
#if	defined(__USING_UNKNOWN__) || !defined(__USING_SJIS__)
		if (ygg_string::npos != ygg_invalid_name_chars_b.find_first_of(X.substr(1))) {
			return ygg_invalid_name_b;
		}
#endif
#if	defined(__USING_UNKNOWN__)
	}
#endif
	return no_error;
}


//	������u�� [ �Y ]
ygg_string & ygg_utility::ygg_string_replace(
		ygg_string &body,
		const ygg_string &X_search, const ygg_string &X_replace) {

	for(ygg_string::size_type p = ygg_string::npos, search_length = X_search.length();
		ygg_string::npos != (p = body.rfind(X_search, p));
		p -= search_length) {
		body.replace(p, search_length, X_replace);
	}

	return body;													//	return
}

ygg_string ygg_utility::create_line(ygg_node X_list, const ygg_string &separator) {
	ygg_string line_string;
	for(int i = 0; X_list[i].is_valid(); ++i) {
		line_string += separator + X_list[i].get_value().get_string();
	}
	if (separator.length() <= line_string.length()) {
		return line_string.substr(separator.length());				//	return
	} else {
		return line_string;											//	return
	}
}



}	//	namespace ygg_utility


ygg_value & ygg_value::set_int(int X) {
#ifdef __USING_STRINGSTREAM___
	std::ostringstream strstr;
	strstr << X;
	value = strstr.str();
#else
	using namespace std;
	char buffer[32];
	sprintf(buffer, "%d",X);
	value = buffer;
#endif
	return *this;													//	return
}
ygg_value & ygg_value::set_double(double X) {
#ifdef __USING_STRINGSTREAM___
	std::ostringstream strstr;
	strstr << X;
	value = strstr.str();
#else
	using namespace std;
	char buffer[32];
	sprintf(buffer, "%f",X);
	value = buffer;
#endif
	return *this;													//	return
}

/******************************************************************************
	��������                          cuppa                         ��������
	��������                 http://www.unittest.org/               ��������
******************************************************************************/

using namespace ygg_utility;

//
//	��ygg�m�[�h�z���_
//
ygg_node & ygg_node::self_exile() {
	for(ygg_iterator i = begin(); i.is_not_end(); ++i) {
		ygg_node parent = i->get_parent();
		assert(ygg_node_type::empty != parent.get_type());
		if (ygg_node_type::empty != parent.get_type()) {
			parent.exile_node(*i);
		}
	}
	return *this;
}

ygg_node & ygg_node::vanish() {
	if (body.is_not_null()) {
	//	��m�[�h�ł͂Ȃ�...
		for(ygg_iterator i = begin(); i.is_not_end(); ++i) {
			if (i->is_live()) {
			//	�S�[�X�g�ł͂Ȃ�...
				ygg_node parent = i->get_parent();
				if (ygg_node_type::empty != parent.get_type()) {
				//	�e�m�[�h�����݂���...
					//	�e�m�[�h����≏����
					parent.exile_node(*i);
				}
			}
		}
		//	�n���h���������(�K���������̃^�C�~���O�Ńm�[�h�������Ă����Ƃ͌���Ȃ�)
		this->operator=(NULL);
	}
	return *this;													//	return
}
ygg_string ygg_node::get_path() const {
	if (body.is_null()) {
	//	��m�[�h...
		return "/null()";											//	return
	}

	ygg_string path;
	ygg_string this_path_name = get_path_name();
	ygg_string index;
	ygg_node parent = body->get_parent();
	if (ygg_node_type::empty != parent.get_type()) {
	//	�e�m�[�h�����݂���...

		//	�e�m�[�h�܂ł̃p�X���擾
		path = parent.get_path();
		if (ygg_term::path_root != path){
			path += ygg_term::path_dir;
		}

		//	�C���f�b�N�X�̏���
		ygg_node X_list = parent[this_path_name];
		int size = X_list.get_size();
		assert(0 < size);
		if (1 < size) {
			for(int i = 0; i < size; ++i) {
				if (this->body == X_list[i].body) {
					char number[2];
					number[0] = '0' +i, number[1] = '\x0';
					index = ygg_term::path_parenthesis_begin
							+number
							+ygg_term::path_parenthesis_end;
					break;											//	break
				}
				assert(i < size -1);
			}
			assert(0 < index.length());
		}
	} else {
	//	�e�m�[�h�����݂��Ȃ�...
		path = ygg_term::path_root;
	}
	path += this_path_name +index;
	return path;													//	return
}
ygg_node & ygg_node::purge() {

	//	����
	if (!is_live()) {
		return vanish();											//	return
	}

	//	�m�[�h��ʂ̎擾
	ygg_string type = get_type();

	if (type == ygg_node_type::empty ||
		type == ygg_node_type::comment ||
		type == ygg_node_type::attribute) {
	//	���ɋ� �������� �R�����g�m�[�h�E�����m�[�h(�M��悤���Ȃ��̂ŕ��u)
		return *this;												//	return
	}

	if (ygg_node_type::text == type) {
	//	�e�L�X�g�m�[�h...
		if (ygg_term::empty != get_value().get_string()) {
			return *this;											//	return
		} else {
		//	���e����̏ꍇ�̓o�j�b�V��
			return vanish();										//	return
		}
	}

	if (type == ygg_node_type::root ||
		type == ygg_node_type::element) {
	//	�v�f�m�[�h...
		ygg_node pre_node;
		ygg_node children = operator[](ygg_term::node);
		for(int i = 0; children[i].is_valid(); children[i++].purge()) {
			if (ygg_node_type::text == pre_node.get_type() &&
				ygg_node_type::text == children[i].get_type()) {
				pre_node.set_value(pre_node.get_value().get_string() +children[i].get_value().get_string());
				children[i].set_value(ygg_term::empty);
			}
			pre_node = children[i];
		}
		return *this;												//	return
	}


	if (ygg_node_type::list == type) {
	//	�m�[�h���X�g...
		ygg_node children = operator[](ygg_term::node);
		for(int i = 0; children[i].is_valid(); children[i++].purge());
		switch(get_size()) {
			case 0:
				return vanish();									//	return
			case 1:
				return *this = operator[](0);						//	return
		}
		return *this;												//	return
	}

	assert(false);
	return *this;													//	return
}

ygg_node ygg_node::enum_node(const ygg_string &path_where) {
	ygg_node node_list = ygg_list::create();
	if (match_path(path_where)) {
		node_list.adopt_node(*this);
	}
	ygg_node children = (*this)[ygg_term::node];
	for(int i = 0; children[i].is_valid(); ++i) {
		node_list.adopt_node(children[i].enum_node(path_where));
	}
	return node_list.purge();										//	return
}


//
//	��ygg�m�[�h�N���X
//
ygg_node ygg_node_body::operator[](const ygg_string &path) {
	if ("." == path) {
		return this;												//	return
	} else {
		return NULL;												//	return
	}
}
ygg_node ygg_node_body::operator[](const unsigned index) {
	if (0 == index) {
		return this;												//	return
	} else {
		return NULL;												//	return
	}
}
const int ygg_node_body::get_size() const {
	return 1;
}

ygg_iterator ygg_node_body::begin() {
	assert(false);
	return *(ygg_iterator*)(void*)NULL;								//	return
}
ygg_iterator ygg_node_body::end() {
	assert(false);
	return *(ygg_iterator*)(void*)NULL;								//	return
}
ygg_reverse_iterator ygg_node_body::rbegin() {
	assert(false);
	return *(ygg_reverse_iterator*)(void*)NULL;						//	return
}
ygg_reverse_iterator ygg_node_body::rend() {
	assert(false);
	return *(ygg_reverse_iterator*)(void*)NULL;						//	return
}
ygg_const_iterator ygg_node_body::begin() const {
	assert(false);
	return *(ygg_const_iterator*)(void*)NULL;						//	return
}
ygg_const_iterator ygg_node_body::end() const {
	assert(false);
	return *(ygg_const_iterator*)(void*)NULL;						//	return
}
ygg_const_reverse_iterator ygg_node_body::rbegin() const {
	assert(false);
	return *(ygg_const_reverse_iterator*)(void*)NULL;				//	return
}
ygg_const_reverse_iterator ygg_node_body::rend() const {
	assert(false);
	return *(ygg_const_reverse_iterator*)(void*)NULL;				//	return
}

bool ygg_node_body::match_path(const ygg_string &) const {
	return false;													//	return
}

void ygg_node_body::set_name(const ygg_string &) {
	assert(false);
}
void ygg_node_body::set_value(const ygg_string &) {
	assert(false);
}
void ygg_node_body::adopt_node(ygg_node) {
	assert(false);
}
void ygg_node_body::exile_node(ygg_node) {
	assert(false);
}

#ifdef _DEBUG
bool ygg_node_body::assert_other(const ygg_node &) const {
	return true;													//	return
}
#endif

//
//	��ygg�S�[�X�g�N���X
//
ygg_node ygg_ghost::get_life() const {
	ygg_node life = parent->operator[](path);
	if (life.is_live()) {
		return life;												//	return
	} else {
		return NULL;												//	return
	}
}
ygg_node ygg_ghost::realize() const {
	ygg_node life = get_life();
	if (life.is_live()) {
		return life;												//	return
	}

	ygg_string::size_type p;
	ygg_string parent_path = path;
	while (0 < parent_path.length() && ygg_term::path_dir == parent_path.substr(p = parent_path.length() -1, 1)) {
		PROMOTION(parent_path).substr(0, p);
	}
	ygg_string::size_type px = 0;
	ygg_string::size_type this_p = 0;
	ygg_string this_path = parent_path;
	ygg_string xi[] = {
			ygg_term::path_dir,
			ygg_term::attribute_prefix,
			ygg_term::path_parenthesis_begin,
			ygg_term::path_parenthesis_end};
	for(ygg_string *i = xi; i < ARRAY_END(xi); ++i) {
		while(ygg_string::npos != (p = parent_path.find(*i, this_p))) {
			px = p;
			this_p = px +1;
			this_path = parent_path.substr(this_p);
		}
	}

	if (ygg_term::empty == this_path) {
		assert(false);												//	return
		return parent;
	}
	if (ygg_term::attribute_prefix == parent_path.substr(px, 1)) {
		life = ygg_attribute::create().set_name(this_path);
	} else if (ygg_term::comment_node == this_path) {
		life = ygg_comment::create();
	} else {
		life = ygg_element::create().set_name(this_path);
	}

	parent_path = parent_path.substr(0, px);
	while (0 < parent_path.length() && ygg_term::path_dir == parent_path.substr(p = parent_path.length() -1, 1)) {
		PROMOTION(parent_path).substr(0, p);
	}

	(*parent)[parent_path].adopt_node(life);//	parent->operator[](parent_path).adopt_node(life);

	return life;													//	return
}

bool ygg_ghost::is_live() const {
	return false;													//	return
}
ygg_string ygg_ghost::get_type() const {
	return get_life().get_type();									//	return
}
ygg_string ygg_ghost::get_name() const {
	return get_life().get_name();									//	return
}
ygg_string ygg_ghost::get_value() const {
	return get_life().get_value().get_string();						//	return
}
ygg_string ygg_ghost::get_text() const {
	return get_life().get_text();									//	return
}
ygg_string ygg_ghost::get_xml(const ygg_string &indent) const {
	return get_life().get_xml(indent);								//	return
}
ygg_string ygg_ghost::get_xml_attribute() const {
	return get_life().get_xml_attribute();							//	return
}
ygg_string ygg_ghost::get_sox(const ygg_string &indent) const {
	return get_life().get_sox(indent);								//	return
}
ygg_node ygg_ghost::operator[](const ygg_string &X_path) {
	ygg_string child_path = path;

	if (ygg_term::path_dir != path.substr(path.length() -1)) {
		child_path += ygg_term::path_dir;
	}
	child_path += X_path;

	return ygg_ghost::create(parent, child_path);					//	return
}
ygg_node ygg_ghost::operator[](const unsigned index) {
	return get_life().operator[](index);							//	return
}
const int ygg_ghost::get_size() const {
	return get_life().get_size();									//	return
}

bool ygg_ghost::match_path(const ygg_string &path) const {
	return get_life().match_path(path);								//	return
}

void ygg_ghost::set_name(const ygg_string &X) {
	realize().set_name(X);
}
void ygg_ghost::set_value(const ygg_string &X) {
	realize().set_value(X);
}
void ygg_ghost::adopt_node(ygg_node X) {
	realize().adopt_node(X);
}
void ygg_ghost::exile_node(ygg_node X) {
	get_life().exile_node(X);
}

ygg_node ygg_ghost::clone() const {
	return create(NULL, path);										//	return
}


#ifdef _DEBUG
bool ygg_ghost::assert_other(const ygg_node &X) const {
	return get_life().assert_other(X);								//	return
}
#endif


//
//	��ygg�R�����g�N���X
//
bool ygg_comment::is_live() const {
	return true;													//	return
}
ygg_string ygg_comment::get_type() const {
	return ygg_node_type::comment;									//	return
}
ygg_string ygg_comment::get_name() const {
	return ygg_term::comment_node;									//	return
}
ygg_string ygg_comment::get_value() const {
	return value;													//	return
}
ygg_string ygg_comment::get_text() const {
	return ygg_term::empty;											//	return
}
ygg_string ygg_comment::get_xml(const ygg_string &) const {
	ygg_string X_text = value;
	ygg_utility::ygg_string_replace(X_text, "-->", "--&gt;");
	return ygg_term::comment_lead +X_text +ygg_term::comment_trail;	//	return
}
ygg_string ygg_comment::get_xml_attribute() const {
	return ygg_term::empty;											//	return
}
ygg_string ygg_comment::get_sox(const ygg_string & indent) const {
	ygg_string X_text;
	ygg_string buffer = value;
	ygg_string::size_type p;
	int return_code_length = ygg_term::return_code.length();

	if (ygg_string::npos != (p = buffer.find(ygg_term::return_code))) {
		X_text += indent +buffer.substr(0, p) +ygg_term::comment_trail +ygg_term::return_code;
		buffer = buffer.substr(p +return_code_length);

		while(ygg_string::npos != (p = buffer.find(ygg_term::return_code))) {
			X_text += indent +buffer.substr(0, p) +ygg_term::return_code;
			buffer = buffer.substr(p +return_code_length);
		}
		X_text += indent +buffer +ygg_term::return_code;
	} else {
		X_text = indent +buffer +ygg_term::comment_trail +ygg_term::return_code;
	}

	return X_text;													//	return
//	return indent +value +ygg_term::comment_trail +ygg_term::return_code;
}

bool ygg_comment::match_path(const ygg_string &path) const {
	return
		path == ygg_term::node ||
		path == ygg_term::comment_node;								//	return
}

void ygg_comment::set_value(const ygg_string &X) {
	value = X;
}

ygg_node ygg_comment::clone() const {
	return create().set_value(value);								//	return
}

//
//	��ygg�e�L�X�g�N���X
//
bool ygg_text::is_live() const {
	return true;													//	return
}
ygg_string ygg_text::get_type() const {
	return ygg_node_type::text;										//	return
}
ygg_string ygg_text::get_name() const {
	return ygg_term::text_node;										//	return
}
ygg_string ygg_text::get_value() const {
	return value;													//	return
}
ygg_string ygg_text::get_text() const {
	return value;													//	return
}
ygg_string ygg_text::get_xml(const ygg_string &) const {
	return encode_xml(xml_trim(value));								//	return
}
ygg_string ygg_text::get_xml_attribute() const {
	return ygg_term::empty;											//	return
}
ygg_string ygg_text::get_sox(const ygg_string & indent) const {
	return encode_sox(indent, value) +ygg_term::return_code;		//	return
}

bool ygg_text::match_path(const ygg_string &path) const {
	return
		path == ygg_term::node ||
		path == ygg_term::text_node;								//	return
}

void ygg_text::set_value(const ygg_string &X) {
	value = X;
}

ygg_node ygg_text::clone() const {
	return create().set_value(value);								//	return
}

//
//	��ygg�����N���X
//
bool ygg_attribute::is_live() const {
	return true;													//	return
}
ygg_string ygg_attribute::get_type() const {
	return ygg_node_type::attribute;								//	return
}
ygg_string ygg_attribute::get_name() const {
	assert(name == encode_xml(name));
	assert(name == encode_attribute(name));
	return name;													//	return
}
ygg_string ygg_attribute::get_value() const {
	return value;													//	return
}
ygg_string ygg_attribute::get_text() const {
	return ygg_term::empty;											//	return
}
ygg_string ygg_attribute::get_xml(const ygg_string &) const {
	assert(name == encode_xml(name));
	assert(name == encode_attribute(name));
	return ygg_term::empty;											//	return
}
ygg_string ygg_attribute::get_xml_attribute() const {
	assert(name == encode_xml(name));
	assert(name == encode_attribute(name));
	return " " +name +"=\"" +encode_attribute(value) +"\"";			//	return
}
ygg_string ygg_attribute::get_sox(const ygg_string & indent) const {
	return indent +name
		+ "=" +encode_attribute(value) +ygg_term::return_code;		//	return
}

bool ygg_attribute::match_path(const ygg_string &path) const {
	return
		path == ygg_term::node ||
		path == ygg_term::attribute_node ||
		path == ygg_term::attribute_prefix +ygg_term::wildcard ||
		path == ygg_term::attribute_prefix +name;					//	return
}

void ygg_attribute::set_name(const ygg_string &X) {
	name = X;
}
void ygg_attribute::set_value(const ygg_string &X) {
	value = X;
}

ygg_node ygg_attribute::clone() const {
	return create().set_name(name).set_value(value);				//	return
}

//
//	��ygg�v���g���X�g�N���X
//
bool ygg_proto_list::is_live() const {
	return true;													//	return
}
ygg_string ygg_proto_list::get_value() const {
	return get_text();												//	return
}
//  [ �Y ]
ygg_string ygg_proto_list::get_text() const {
	ygg_string X_text;
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		X_text += i->get_text();
	}
	return X_text;													//	return
}
//  [ �Y ]
ygg_string ygg_proto_list::get_xml(const ygg_string & indent) const {
	ygg_string X_text, X_part, X_core;
	bool gap = false;
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		X_part = i->get_xml(indent);
		if (0 < X_part.length()) {
			X_core = both_trim(X_part);
			if (0 < X_core.length()) {
				if (!gap && 0 < X_part.find_first_not_of(ygg_term::white_space)) {
					X_core = ygg_term::return_code +indent +X_core;
				}
				ygg_string::size_type end_pos = X_part.find_last_not_of(ygg_term::white_space);
				if (ygg_string::npos != end_pos && end_pos +1 < X_part.length()) {
					gap = true;
					X_core += ygg_term::return_code +indent;
				} else {
					gap = false;
				}
			} else {
				if (!gap) {
					gap = true;
					X_core += ygg_term::return_code +indent;
				}
			}
			X_text += X_core;
		}
	}
	return X_text;													//	return
}
//  [ �Y ]
ygg_string ygg_proto_list::get_xml_attribute() const {
	ygg_string X_text;
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		X_text += i->get_xml_attribute();
	}
	return X_text;													//	return
}
//  [ �Y ]
ygg_string ygg_proto_list::get_sox(const ygg_string & indent) const {
	ygg_string X_text;
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		X_text += i->get_sox(indent);
	}
	return X_text;													//	return
}

void ygg_proto_list::adopt_node(ygg_node X) {
	adopt_child(X);
}
void ygg_proto_list::exile_node(ygg_node X) {
	exile_child(X);
}

//	���X�g�ւ̃m�[�h�ǉ� [ �Y ]
ygg_proto_list * ygg_proto_list::adopt_child(ygg_node X_node) {
	for(int i = 0; X_node[i].is_valid(); ++i) {
#ifdef _DEBUG
		assert_other(X_node);
#endif
		body.insert(body.end(), X_node[i]);
	}
	return this;													//	return
}
//	���X�g����m�[�h�ǉ��폜 [ �Y ]
ygg_proto_list * ygg_proto_list::exile_child(ygg_node X_node) {
	for(int i = 0; X_node[i].is_valid(); ++i) {
		body.remove(X_node[i]);
	}
	return this;													//	return
}


#ifdef _DEBUG
bool ygg_proto_list::assert_other(const ygg_node &X) const {
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		if (!i->assert_other(X)) {
			return false;											//	return
		}
	}
	return (const ygg_node_body*)this != (const ygg_node_body*)X.body;			//	return
}
#endif

//
//	��ygg���X�g�N���X
//
ygg_string ygg_list::get_type() const {
	return ygg_node_type::list;										//	return
}
ygg_string ygg_list::get_name() const {
//	return ygg_term::list_node;
	return ygg_term::empty;											//	return
}
//  [ �Y ]
ygg_node ygg_list::operator[](const ygg_string &path) {
	if (ygg_term::path_last_index == path) {
		return *body.rbegin();
	}
	ygg_node X_list = ygg_list::create();
	for(ygg_list_type::iterator i = body.begin(); i != body.end(); ++i) {
		X_list.adopt_node(i->operator[](path));
	}
	switch(X_list.get_size()) {

		case 0:
			return NULL;											//	return

		case 1:
			return X_list[0];										//	return

		default:
			return X_list;											//	return

	}
}
//  [ �Y ]
ygg_node ygg_list::operator[](const unsigned index) {
	if (index < body.size()) {
		ygg_list_type::iterator i = body.begin();
		std::advance(i, index);
		return *i;													//	return
	} else {
		return NULL;												//	return
	}
}
const int ygg_list::get_size() const {
	return body.size();												//	return
}
ygg_iterator ygg_list::begin() {
	return ygg_iterator(this, body.begin());						//	return
}
ygg_iterator ygg_list::end() {
	return ygg_iterator(this, body.end());							//	return
}
ygg_reverse_iterator ygg_list::rbegin() {
	return ygg_reverse_iterator(get_shell(), body.rbegin());		//	return
}
ygg_reverse_iterator ygg_list::rend() {
	return ygg_reverse_iterator(get_shell(), body.rend());			//	return
}
ygg_const_iterator ygg_list::begin() const {
	return ygg_const_iterator(get_shell(), body.begin());			//	return
}
ygg_const_iterator ygg_list::end() const {
	return ygg_const_iterator(get_shell(), body.end());				//	return
}
ygg_const_reverse_iterator ygg_list::rbegin() const {
	return ygg_const_reverse_iterator(get_shell(), body.rbegin());	//	return
}
ygg_const_reverse_iterator ygg_list::rend() const {
	return ygg_const_reverse_iterator(get_shell(), body.rend());	//	return
}

bool ygg_list::match_path(const ygg_string &path) const {
	assert(false);
	//assert(("ygg_list::match_path ���Ă΂��悤�Ȃ��Ƃ͂Ȃ��͂��B�ǂ����Ƀo�O�ɂ�����Ǝv����B", false));
	return
		path == ygg_term::node ||
		path == ygg_term::list_node;								//	return
}

ygg_node ygg_list::clone() const {
	ygg_node X_clone = create();
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		X_clone.adopt_node(i->clone());
	}
	return X_clone;													//	return
}

//
//	��ygg���N���X
//
ygg_string ygg_root::get_type() const {
	return ygg_node_type::root;										//	return
}
ygg_string ygg_root::get_name() const {
	return ygg_term::empty;											//	return
}
//  [ �Y ]
ygg_node ygg_root::operator[](const ygg_string &path) {

	assert(1 == ygg_term::path_dir.length());
	assert(1 == ygg_term::attribute_prefix.length());
	assert(1 == ygg_term::path_parenthesis_begin.length());
	assert(1 == ygg_term::path_parenthesis_end.length());

	if (ygg_term::empty == path || ygg_term::path_this == path) {
		return this;												//	return
	}
	if (ygg_term::node == path) {
		ygg_node X_list = ygg_list::create();
		for(ygg_list_type::iterator i = body.begin(); i != body.end(); ++i) {
			X_list.adopt_node(*i);
		}
		return X_list;												//	return
	}

	if (0 == path.find(ygg_term::path_wherever)) {
		return operator[](ygg_term::path_root).enum_node(path.substr(ygg_term::path_wherever.length()));
																	//	return
	}

	if (0 == path.find(ygg_term::path_parent)) {
		ygg_node &X_parent = get_parent();
		if (ygg_node(NULL) != X_parent) {
			return X_parent.operator[](path.substr(1));				//	return
		} else {
			return NULL;											//	return
		}
	}

	if (0 == path.find(ygg_term::path_this)) {
		if (0 == path.find(ygg_term::path_this__wherever)) {
			return get_shell().enum_node(path.substr(ygg_term::path_this__wherever.length()));
																	//	return
		} else if (0 == path.find(ygg_term::path_this_element)) {
			return operator[](path.substr(ygg_term::path_this_element.length()));
																	//	return
		} else {
			return operator[](path.substr(ygg_term::path_this.length()));
																	//	return
		}
	}

	if (0 == path.find(ygg_term::path_dir)) {
		ygg_node &X_parent = get_parent();
		if (ygg_node(NULL) != X_parent) {
			return X_parent.operator[](path);
																	//	return
		} else {
			return operator[](path.substr(ygg_term::path_dir.length()));
																	//	return
		}
	}

	ygg_string current_path = path;
	ygg_string next_term = ygg_term::empty;

	if (0 != current_path.find(ygg_term::attribute_prefix)) {

		ygg_string::size_type p;
		ygg_string xi[] = {
				ygg_term::path_dir,
				ygg_term::attribute_prefix,
				ygg_term::path_parenthesis_begin,
				ygg_term::path_parenthesis_end};
		for(ygg_string *i = xi; i < ARRAY_END(xi); ++i) {
			if (ygg_string::npos != (p = current_path.find(*i))) {
				next_term = *i;
				current_path = current_path.substr(0, p);
			}
		}
	}

	ygg_node X_list = ygg_list::create();
	for(ygg_list_type::iterator i = body.begin(); i != body.end(); ++i) {
		if (i->match_path(current_path)) {
			X_list.adopt_node(*i);
		}
	}

	if (ygg_term::path_parenthesis_begin == next_term) {
		assert(ygg_string::npos != path.find(next_term));

		ygg_string index_string = path.substr(path.find(next_term) +1);
		if (0 == index_string.find(ygg_term::path_last_index)) {
			X_list = *X_list.rbegin();
		} else {
			int index = atoi(index_string.c_str());
			X_list = X_list[index];
		}

		assert(ygg_string::npos != path.find(ygg_term::path_parenthesis_end));
		current_path = path.substr(path.find(ygg_term::path_parenthesis_end) +1);
		next_term = ygg_term::empty;
		ygg_string::size_type p;
		ygg_string xi[] = {
				ygg_term::path_dir,
				ygg_term::attribute_prefix};
		for(ygg_string *i = xi; i < ARRAY_END(xi); ++i) {
			if (ygg_string::npos != (p = current_path.find(*i))) {
				next_term = *i;
				current_path = current_path.substr(0, p);
			}
		}
	}

	if (ygg_term::empty != next_term) {
		ygg_string next_path = path;
		if (ygg_term::path_dir != next_term) {
			next_path = next_path.substr(next_path.find(next_term));
		} else {
			next_path = next_path.substr(next_path.find(next_term) +1);
		}

		ygg_node X_list_temp = ygg_list::create();
		for(int i = 0; X_list[i].is_valid(); ++i) {
			X_list_temp.adopt_node(X_list[i][next_path]);
		}
		X_list = X_list_temp;
	}

	switch(X_list.get_size()) {

		case 0:
			return ygg_ghost::create(this, path);					//	return

		case 1:
			return X_list[0];										//	return

		default:
			return X_list;											//	return

	}
}

void ygg_root::adopt_node(ygg_node X_node) {
	for(ygg_iterator i = X_node.begin(); i.is_not_end(); ++i) {
		i->regist_parent(this);
	}
//	for(int i = 0; X_node[i].is_valid(); ++i) {
//		X_node[i].regist_parent(this);
//	}
}
void ygg_root::exile_node(ygg_node X_node) {
	for(ygg_iterator i = X_node.begin(); i.is_not_end(); ++i) {
		i->unregist_parent(this);
	}
//	for(int i = 0; X_node[i].is_valid(); ++i) {
//		X_node[i].unregist_parent(this);
//	}
}

bool ygg_root::match_path(const ygg_string &path) const {
	return
		path == ygg_term::node ||
		path == ygg_term::element_node ||
		path == ygg_term::wildcard;									//	return
}

ygg_node ygg_root::clone() const {
	ygg_node X_clone = create();
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		X_clone.adopt_node(i->clone());
	}
	return X_clone;													//	return
}

//
//	��ygg�v�f�N���X
//
ygg_string ygg_element::get_type() const {
	return ygg_node_type::element;									//	return
}
ygg_string ygg_element::get_name() const {
	assert(name == encode_xml(name));
	assert(name == encode_attribute(name));
	return name;													//	return
}
ygg_string ygg_element::get_xml(const ygg_string & indent) const {
	const ygg_string inner_indent = indent +ygg_term::indent;
	ygg_string X_text = ygg_proto_list::get_xml(inner_indent);
	ygg_string X_attribute = ygg_proto_list::get_xml_attribute();
	if (0 == name.find("?")) {
		X_text = "<" +name +X_attribute +" ?>";
	} else if (ygg_term::empty == X_text) {
		X_text = "<" +name +X_attribute +" />";
	} else {
		ygg_string X_core = both_trim(X_text);
		if (0 < X_text.find_first_not_of(ygg_term::white_space)) {
			X_core = ygg_term::return_code +inner_indent +X_core;
		}
		ygg_string::size_type end_pos = X_text.find_last_not_of(ygg_term::white_space);
		if (ygg_string::npos != end_pos && end_pos +1 < X_text.length()) {
			X_core += ygg_term::return_code +indent;
		}
		X_text = "<" +name +X_attribute +">" +X_core +"</" + name + ">";
	}
	return X_text;													//	return
}
ygg_string ygg_element::get_xml_attribute() const {
	return ygg_term::empty;											//	return
}
ygg_string ygg_element::get_sox(const ygg_string & indent) const {
	const ygg_string child_indent = indent +ygg_term::indent;
	const ygg_list_type &X_list = body;
	ygg_string X_attribute;
	ygg_string X_text;

	for(ygg_list_type::const_iterator i = X_list.begin(); i != X_list.end(); ++i) {
		if (ygg_node_type::attribute == i->get_type()) {
			X_attribute += i->get_sox(child_indent);
		} else {
			X_text += i->get_sox(child_indent);
		}
	}

	X_text = indent +name +">" +ygg_term::return_code
		+X_attribute
		+X_text;
	return X_text;													//	return
}

bool ygg_element::match_path(const ygg_string &path) const {
#ifdef __REJECT_PROCESSING_INSTRUCTION__
	return
		path == ygg_term::node ||
		path == ygg_term::element_node ||
		path == ygg_term::wildcard ||
		path == name;												//	return
#else
	return
		path == ygg_term::node ||
		(ygg_string::npos != name.find("!") ?
			path == ygg_term::element_node ||
			path == ygg_term::wildcard:
			path == ygg_term::processing_instruction_node) ||
		path == name;												//	return
#endif
}

void ygg_element::set_name(const ygg_string &X) {
	name = X;
}
void ygg_element::set_value(const ygg_string &X) {
	ygg_list_type::iterator i = body.begin();
	while(i != body.end()) {
		if (ygg_node_type::attribute != i->get_type()) {
			i++->vanish();
		} else {
			++i;
		}
	}
	adopt_node(ygg_text::create().set_value(X));
}

ygg_node ygg_element::clone() const {
	ygg_node X_clone = create().set_name(name);
	for(ygg_list_type::const_iterator i = body.begin(); i != body.end(); ++i) {
		X_clone.adopt_node(i->clone());
	}
	return X_clone;													//	return
}

/******************************************************************************
	��������                 Trick Library 'dagger'                 ��������
	��������             http://tricklib.com/cxx/dagger/            ��������
******************************************************************************/

//
//	��ygg_position
//
ygg_position & ygg_position::operator+=(const ygg_string &X) {
	ygg_string X_text = X;
	ygg_string::size_type p;
	const ygg_string::size_type return_code_length = ygg_term::return_code.length();
	while(ygg_string::npos != (p = X_text.find(ygg_term::return_code))) {
		next_line();
		PROMOTION(X_text).substr(p +return_code_length);
	}
	row += X_text.length();
	return *this;
}


//
//	��SAX�n���h���[��{�N���X
//
void sax_handler::on_error(ygg_error*) {}
void sax_handler::start_document(ygg_node) {}
void sax_handler::end_document(ygg_node&) {}
void sax_handler::start_element(ygg_node) {}
void sax_handler::end_element(ygg_node) {}
void sax_handler::catch_text(ygg_node) {}
void sax_handler::catch_comment(ygg_node) {}


//
//	��ygg�p�[�U�N���X
//
ygg_parser * ygg_parser::set_sax_handler(sax_handler *X_sax) {
	sax = X_sax;
	AD_LIBTIUM(sax).set_parser(this);
	return this;													//	return
}

const ygg_error_code ygg_parser::check_name(const ygg_string &type, const ygg_string &X) {
	using namespace ygg_error_term;

	ygg_error_code name_error = ygg_utility::check_name(X);

	const int error_code = name_error.get_code();
	const int sequence_case = name_error.get_sequence_case();

	if (error_code == ygg_broken_char.get_code()) {
	//	�s���ȕ����R�[�h...
		raise_error(name_error, "�s���ȕ����R�[�h������܂��B");
	} else if (error_code == ygg_invalid_name.get_code()) {
	//	�s�K�؂Ȗ��O...
		if (sequence_case == ygg_invalid_name_a.get_sequence_case()) {
			raise_error(name_error, "�s�K�؂�" +type +"(" +X +")�ł��B(���̕�����" +type +"�̈ꕶ���ڂɂ͎g�p�ł��܂��� '" +ygg_term::ygg_invalid_name_chars_a +"')");
		} else if (sequence_case == ygg_invalid_name_b.get_sequence_case()) {
			raise_error(name_error, "�s�K�؂�" +type +"(" +X +")�ł��B(���̕�����" +type +"�Ƃ��Ďg�p�ł��܂��� '" +ygg_term::ygg_invalid_name_chars_b +"')");
		} else {
			raise_error(name_error, "�s�K�؂�" +type +"(" +X +")�ł��B");
		}
	} else if (error_code != no_error.get_code()) {
	//	���̑��̃G���[...
		raise_error(name_error);
	}
	//	sax->on_error ���ŃG���[���N���A���ꂽ�ꍇ�ɂ͋��s����
	return parse_error;
}


//
//	��SOX�p�[�U�N���X
//
sox_parser & sox_parser::init_root() {
	root = ygg_root::create();
	hot_element = root;
	last_node = root;
	indent_node_list = ygg_list::create();
	indent_node_list.adopt_node(root);
	anchor_indent = -1;
	anchor_position.clear();
	hot_position.clear();
	parse_error.clear();

	AD_LIBTIUM(sax).start_document(root);

	return *this;													//	return
}
sox_parser & sox_parser::parse_line(const ygg_string &X_line) {
	//	�������񖢏����o�b�t�@�Ɏc���Ă�����������
	flush();
	//	���̌�͊ۓ���
	parse(X_line);

	//	���ݍs�̃J�E���g
	anchor_position.next_line();

	return *this;													//	return
}
sox_parser & sox_parser::parse_string(const ygg_string &X_text) {
	//	�Ƃ肠�����������o�b�t�@�ɒǉ�
	unparsed_buffer += X_text;
	//	�s�P�ʂŉ�͏����ɓ�����
	int return_code_length = ygg_term::return_code.length();
	ygg_string::size_type p;
	while(ygg_string::npos != (p = unparsed_buffer.find(ygg_term::return_code))) {
		parse(unparsed_buffer.substr(0, p));
		if (parse_error.is_error()) {
			break;													//	break
		}

		//	���ݍs�̃J�E���g
		anchor_position.next_line();

		unparsed_buffer = unparsed_buffer.substr(p +return_code_length);
	}

	return *this;													//	return
}
sox_parser & sox_parser::flush() {
	//	�������o�b�t�@�Ɏc���Ă�������Ώ�������
	if (ygg_term::empty != unparsed_buffer) {
		parse(unparsed_buffer);
		unparsed_buffer = ygg_term::empty;
	}

	return *this;													//	return
}
sox_parser & sox_parser::end_stream() {
	flush();

	if (sax) {
		catch_event(last_node);
		if (ygg_node_type::element == hot_element.get_type()) {
			for(ygg_node node = hot_element;
				node.is_valid() && node != root;
				PROMOTION(node).get_parent()) {

				sax->end_element(node);
			}
		}
		sax->end_document(root);
	}

	return *this;													//	return
}


void sox_parser::parse(const ygg_string &X_text) {

	ygg_string X_parse = X_text;

	ygg_string::size_type p;
	ygg_string name;
	ygg_string value;

	//	���ɃG���[���������Ă�����ǂ��Ԃ�
	if (parse_error.is_error()) {
		return;														//	return
	}

	//	���s�R�[�h���܂��c���Ă���Ύ�菜��
	const char *xi[] = {"\x0a", "\x0d"};
	for(const char **i = xi; i < ARRAY_END(xi); ++i) {
		if (ygg_string::npos != (p = X_parse.find(*i))) {
			PROMOTION(X_parse).substr(0, p);
		}
	}

	//	�C���f���g�𐔂���
	int indent_count = 0;
	int indent_length = ygg_term::indent.length();
	while(ygg_term::indent == X_parse.substr(0, indent_length)) {
		PROMOTION(X_parse).substr(indent_length);
		++indent_count;
		anchor_position.row += indent_length;
	}
	if (0 == X_parse.length()) {
	//	��s����...
		return;														//	return
	}

	//
	//	�A���J�[�C���f���g�̍��������ɏ������s��
	//

	while(indent_count <= anchor_indent) {
		//	�e�G�������g�֍~���
		--anchor_indent;
		assert(root != hot_element);
		assert(ygg_node_type::empty != hot_element.get_type());

		assert(1 < indent_node_list.get_size());
		indent_node_list.exile_node(indent_node_list[indent_node_list.get_size() -1]);
		assert(0 < indent_node_list.get_size());
		ygg_node indent_node = indent_node_list[indent_node_list.get_size() -1];

		if (sax) {
			catch_event(last_node);
			if (ygg_node_type::element == hot_element.get_type()) {
				for(ygg_node node = hot_element;
					node.is_valid() && node != indent_node;
					PROMOTION(node).get_parent()) {

					sax->end_element(node);
				}
			}
		}
		hot_element = indent_node;
//		last_node = hot_element;
		last_node = NULL;

//		hot_element = hot_element.get_parent();
		assert(ygg_node_type::empty != hot_element.get_type());
	}


	//
	//	�m�[�h�̌��o
	//

	if (ygg_node_type::comment == last_node.get_type()) {
		//	�R�����g�̑���...
		last_node.set_value(
			last_node.get_value().get_string() +ygg_term::return_code
			+make_indent(indent_count -(anchor_indent +1))
			 +X_parse);
		return;														//	return
	}

	// �����Ő[������C���f���g�̃G���[���o
	if (anchor_indent +1 < indent_count) {
		raise_error(ygg_error_term::sox_deep_indent, "�C���f���g���[�����܂��B");
		return;														//	return
	}

	if (ygg_string::npos != (p = X_parse.rfind(ygg_term::comment_trail))) {
		//	�R�����g
		catch_event(last_node);
		value = X_parse.substr(0, p);
		last_node = ygg_comment::create().set_value(value);
		hot_element.adopt_node(last_node);

		hot_element = last_node;
		indent_node_list.adopt_node(last_node);
		assert(anchor_indent +1 == indent_count);
		anchor_indent = indent_count;

		X_parse = X_parse.substr(p +ygg_term::comment_trail.length());
		while(ygg_term::indent == X_parse.substr(0, indent_length)) {
			PROMOTION(X_parse).substr(indent_length);
		}
		if (0 < X_parse.length()) {
			last_node.set_value(
				value +ygg_term::return_code
				+ygg_term::indent +X_parse);
		}

		return;														//	return
	}

	while(ygg_string::npos != (p = X_parse.find(ygg_term::element_trail))) {
		//	�G�������g
		catch_event(last_node);
		name = X_parse.substr(0, p);

		if (ygg_parser::check_element_name(name).is_error()) {
			return;
		}

		last_node = ygg_element::create().set_name(name);
		hot_element.adopt_node(last_node);

		hot_element = last_node;

		if (anchor_indent < indent_count) {
			assert(anchor_indent +1 == indent_count);
			anchor_indent = indent_count;
			indent_node_list.adopt_node(last_node);
		}

		X_parse = X_parse.substr(p +ygg_term::element_trail.length());
		while(ygg_term::indent == X_parse.substr(0, indent_length)) {
			PROMOTION(X_parse).substr(indent_length);
		}

	}

	if (0 == X_parse.length()) {
		return;														//	return
	}

	if (ygg_string::npos != (p = X_parse.find(ygg_term::equal))) {
		//	�A�g���r���[�g
		name = X_parse.substr(0, p);

		if (ygg_parser::check_attribute_name(name).is_error()) {
			return;
		} else if (hot_element["@"+name].is_valid()) {
			raise_error(ygg_error_term::duplication_attribute, name +"�������d�����Ă��܂��B");
			//	sax->on_error ���ŃG���[���N���A���ꂽ�ꍇ�͋��s����...
			if (parse_error.is_error()) {
				return;
			}
		}

		value = decode_attribute(X_parse.substr(p +ygg_term::equal.length()));
//		last_node = ygg_attribute::create().set_name(name).set_value(value);
//		hot_element.adopt_node(last_node);
		hot_element.adopt_node(ygg_attribute::create().set_name(name).set_value(value));
	} else {
		//	�e�L�X�g
		if (ygg_node_type::text != last_node.get_type()) {
			//	�V�K
			catch_event(last_node);
			value = decode_xml(X_parse);
			last_node = ygg_text::create().set_value(value);
			hot_element.adopt_node(last_node);
		} else {
			//	����...
			last_node.set_value(
				last_node.get_value().get_string() +ygg_term::return_code
				+make_indent(indent_count -(anchor_indent +2))
				 +X_parse);
		}
	}
}
void sox_parser::catch_event(ygg_node node) {
	if (sax) {
		ygg_string type = node.get_type();
		if (ygg_node_type::text == type) {
			sax->catch_text(node);
		} else if (ygg_node_type::comment == type) {
			sax->catch_comment(node);
		} else if (ygg_node_type::element == type) {
			sax->start_element(node);
		}
	}
}

//
//	��XML�p�[�U�N���X
//
xml_parser & xml_parser::init_root() {
	root = ygg_root::create();
	hot_element = root;
	last_node = root;
	anchor_position.clear();
	hot_position.clear();
	parse_error.clear();

	AD_LIBTIUM(sax).start_document(root);

	return *this;													//	return
}
xml_parser & xml_parser::parse_line(const ygg_string &X_line) {
	if (ygg_string::npos != X_line.find(ygg_term::return_code)) {
		parse_string(X_line);
	} else {
		parse_string(X_line +ygg_term::return_code);
	}
	return *this;													//	return
}
xml_parser & xml_parser::parse_string(const ygg_string &X_text) {
	//	�Ƃ肠�����������o�b�t�@�ɒǉ�
	unparsed_buffer += X_text;

	//	���ɃG���[���������Ă�����ǂ��Ԃ�
	if (parse_error.is_error()) {
		return *this;												//	return
	}

	ygg_string::size_type p, p_end;
	ygg_string value;

	while (ygg_string::npos != (p = unparsed_buffer.find(ygg_term::element_lead))) {

		if (0 != p) {
		//	��Ƀe�L�X�g������...
			ygg_string source_text = unparsed_buffer.substr(0, p);
			value = decode_xml(ygg_utility::xml_trim(source_text));
			if (ygg_node_type::text == last_node.get_type()) {
			//	�����̃e�L�X�g...
				last_node.set_value(last_node.get_value().get_string() +value);
			} else {
			//	�V�K�̃e�L�X�g...
				anchor_position = hot_position;
				last_node = ygg_text::create().set_value(value);
				hot_element.adopt_node(last_node);
			}
			//	����������\��������̂ł��̒i�K�ł̓C�x���g�𔭐������Ȃ��B
			//	AD_LIBTIUM(sax).catch_text(last_node);
			hot_position += source_text;
			PROMOTION(unparsed_buffer).substr(p), p = 0;
		}

		if (p == unparsed_buffer.find(ygg_term::comment_lead)) {
			//	�R�����g...
			p_end = unparsed_buffer.find(ygg_term::comment_trail, p);
			if (ygg_string::npos != p_end) {
				//	�R�����g��"��"�𔭌�...

				//	���O�̃m�[�h���e�L�X�g�m�[�h�Ȃ�e�L�X�g�C�x���g�𔭐��B
				if (ygg_node_type::text == last_node.get_type()) {
					AD_LIBTIUM(sax).catch_text(last_node);
				}

				value = unparsed_buffer.substr(0, p_end).substr(p +ygg_term::comment_lead.length());
				last_node = ygg_comment::create().set_value(value);
				hot_element.adopt_node(last_node);
				AD_LIBTIUM(sax).catch_comment(last_node);
				hot_position += unparsed_buffer.substr(0, p_end +ygg_term::comment_trail.length());
				PROMOTION(unparsed_buffer).substr(p_end +ygg_term::comment_trail.length());
			} else {
				break;												//	break
			}
		} else if (p == unparsed_buffer.find(ygg_term::cdata_lead)) {
			//	CDATA�e�L�X�g...
			p_end = unparsed_buffer.find(ygg_term::cdata_trail, p);
			if (ygg_string::npos != p_end) {
				//	CDATA��"��"�𔭌�...

				value = unparsed_buffer.substr(0, p_end).substr(p +ygg_term::cdata_lead.length());
				if (ygg_node_type::text == last_node.get_type()) {
					last_node.set_value(last_node.get_value().get_string() +value);
				} else {
					last_node = ygg_text::create().set_value(value);
					hot_element.adopt_node(last_node);
				}
				//	����������\��������̂ł��̒i�K�ł̓C�x���g�𔭐������Ȃ��B
				//	AD_LIBTIUM(sax).catch_text(last_node);
				hot_position += unparsed_buffer.substr(0, p_end +ygg_term::cdata_trail.length());
				PROMOTION(unparsed_buffer).substr(p_end +ygg_term::cdata_trail.length());
			} else {
				break;												//	break
			}
		} else {
			//	�G�������g...
			p_end = unparsed_buffer.find(ygg_term::element_trail, p);
			if (ygg_string::npos != p_end) {
				//	�G�������g��"��"�𔭌�...

				//	���O�̃m�[�h���e�L�X�g�m�[�h�Ȃ�e�L�X�g�C�x���g�𔭐��B
				if (ygg_node_type::text == last_node.get_type()) {
					AD_LIBTIUM(sax).catch_text(last_node);
				}

				//	�^�O���̃e�L�X�g
				ygg_string element_string = unparsed_buffer.substr(0, p_end).substr(p +ygg_term::element_lead.length());

				//
				anchor_position = hot_position;
				hot_position += unparsed_buffer.substr(0, p_end +ygg_term::element_trail.length());
				PROMOTION(unparsed_buffer).substr(p_end +ygg_term::element_trail.length());

				//	"/" �̃`�F�b�N

				ygg_string::size_type p_lead_sign = 0;
				ygg_string::size_type p_trail_sign = element_string.length() -ygg_term::element_sign.length();

				bool lead_sign	= p_lead_sign == element_string.find(ygg_term::element_sign, p_lead_sign);
				bool trail_sign	= p_trail_sign == element_string.find(ygg_term::element_sign, p_trail_sign);

				if (trail_sign)	PROMOTION(element_string).substr(0, p_trail_sign);
				if (lead_sign)	PROMOTION(element_string).substr(ygg_term::element_sign.length());

#				ifdef __REJECT_PROCESSING_INSTRUCTION__
#				else
					bool is_pi_node = false;
#				endif

				if (!lead_sign && !trail_sign &&
					p_lead_sign == element_string.find("?", p_lead_sign) &&
					p_trail_sign == element_string.find("?", p_trail_sign)) {
				//	<?�`?> ...
#					ifdef __REJECT_PROCESSING_INSTRUCTION__
						//	�ǂݔ�΂�
						continue;									//	continue
#					else
						//	����ȃG�������g�Ƃ��ď���
						PROMOTION(element_string).substr(0, p_trail_sign);
						trail_sign = true;
						is_pi_node = true;
#					endif
				}

				//	�v�f���̎擾
				ygg_string::size_type p_name_end = element_string.find_first_of(ygg_term::white_space);
				ygg_string element_name = element_string.substr(0, p_name_end);
				if (ygg_string::npos != p_name_end) {
					PROMOTION(element_string).substr(p_name_end);
				} else {
					PROMOTION(element_string) = ygg_term::empty;
				}

				//	�擪��"/"���������
				if (!lead_sign) {
#				ifdef __REJECT_PROCESSING_INSTRUCTION__
					if (ygg_parser::check_element_name(element_name).is_error()) {
#				else
					if (ygg_parser::check_element_name((is_pi_node) ? element_name.substr(1): element_name).is_error()) {
#				endif
						return *this;								//	return
					}
					last_node = ygg_element::create().set_name(element_name);
					hot_element.adopt_node(last_node);
					hot_element = last_node;

					//	�����̎擾...
					while(true) {

						ygg_string::size_type p_anchor = element_string.find_first_not_of(ygg_term::white_space);
						if (ygg_string::npos == p_anchor) {
							break;									//	break
						}
						PROMOTION(element_string).substr(p_anchor);

						ygg_string::size_type p_name_end = element_string.find_first_of("=" +ygg_term::white_space);
						ygg_string attribute_name = element_string.substr(0, p_name_end);
						if (ygg_parser::check_attribute_name(attribute_name).is_error()) {
							return *this;							//	return
						} else if (hot_element["@"+attribute_name].is_valid()) {
							raise_error(ygg_error_term::duplication_attribute, attribute_name +"�������d�����Ă��܂��B");
							//	sax->on_error ���ŃG���[���N���A���ꂽ�ꍇ�͋��s����...
							if (parse_error.is_error()) {
								return *this;						//	return
							}
						}

						ygg_string hedge_symbol;
						ygg_string::size_type p_value;
						ygg_string::size_type p_value_a = element_string.find("\"", p_name_end);
						ygg_string::size_type p_value_b = element_string.find("\'", p_name_end);
						if (ygg_string::npos == p_value_b ||
							(ygg_string::npos != p_value_a && p_value_a <= p_value_b)) {

							hedge_symbol = "\"";
							p_value = p_value_a;
						} else {
							hedge_symbol = "\'";
							p_value = p_value_b;
						}

						ygg_string::size_type p_value_end = element_string.find(hedge_symbol, p_value +1);

						value = element_string.substr(0, p_value_end).substr(p_value +1);

						hot_element.adopt_node(ygg_attribute::create().set_name(attribute_name).set_value(value));

						PROMOTION(element_string).substr(p_value_end +1);
					}

					//	�G�������g�J�n�C�x���g�𔭐��B
					AD_LIBTIUM(sax).start_element(hot_element);
				}

				//	�擪�������͖�����"/"���L���
				if (lead_sign || trail_sign) {
					if (element_name != hot_element.get_name()) {
						if (root != hot_element) {
							raise_error(ygg_error_term::unmatch_tags_a, "�J�n�^�O<" +hot_element.get_name() +">�ƏI���^�O</" +element_name +">���}�b�`���Ă��܂���B");
						} else {
							raise_error(ygg_error_term::unmatch_tags_b, "�����Ȃ�I���^�O<" +element_name +">����n�܂��Ă��܂��B");
						}
						if (parse_error.is_error()) {
							return *this;							//	return
						}
					}

					//	end_element ���� vanish �����Ɛe��������Ȃ��Ȃ�̂ł����Ŏ擾�B
					ygg_node parent = hot_element.get_parent();

					//	�G�������g�I���C�x���g�𔭐��B
					AD_LIBTIUM(sax).end_element(hot_element);

					hot_element = parent;
					last_node = NULL;
				}

			} else {
				break;												//	break
			}
		}
	}
	return *this;													//	return
}
xml_parser & xml_parser::flush() {
	//	xml�łł͉������Ȃ��B
	return *this;													//	return
}
xml_parser & xml_parser::end_stream() {
	flush();

	if (sax) {
		if (ygg_node_type::text == last_node.get_type()) {
			sax->catch_text(last_node);
		}
		sax->end_document(root);
	}

	return *this;													//	return
}

}	//	namespace yggdrasil

/******************************************************************************
	��������                  Wraith the Trickster                  ��������
	�������� �`I'll go with heaven's advantage and fool's wisdom.�` ��������
******************************************************************************/

