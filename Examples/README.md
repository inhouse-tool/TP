# Examples

## ����

�ȉ���, ��҂̎莝���� PC �Ōv�����Ă݂����ʂł�.
�����܂Ōv���̎���Ƃ��Ă��l����������.

### Client

�_���R�A��16��, ��r�I[�����\�� PC �ł̌v������](Client/README.md).

### Server

�_���R�A��4��, ��r�I[�ᐫ�\�� PC �ł̌v������](Server/README.md).

<sub>�������̌v����2024�N10�����݂� Windows �ōs��ꂽ���̂ł�.
����[Windows ���d�l��ς���](https://randomascii.wordpress.com/2020/10/04/windows-timer-resolution-the-great-rule-change/)�܂ł�, 
�܂��܂��������͂��ł�.
</sub>


## ����

��L�̌v���̌���, �������X���́F

* �덷���}15.625[ms] ( 1,000/64[s] ) �ȓ��Ɏ��߂����̂Ȃ� [`SetTimer`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-settimer) �͘_�O.
* [`SetWaitableTimer`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-setwaitabletimer) �ł��Ȃ�����ł���.
* ����w�ǂ��l�߂邽�߂ɂ� [`timeBeginPeriod`](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod) �̏��͂��K�v.
* �^�C�}�[������ window message �œ`���邾���ł����̃v���Z�X�� CPU ���Ԃ��������X�L��������.
* �Ȃ̂�, message ��� callback �֐��Ńn���h������������_��̓x�^�[. ( �g���Â炢���� )
* �c�c�Ƃ����悤�ȓw�͂��d�˂��Ƃ����, ���� CPU ���Ԃ��򂤃v���Z�X�����������ς�u����.
* ����, �^�C�}�[�̐��x�����ł̓_����, �^�C�}�[�ɉ����� CPU �̗]�͂��K�v.

�Ƃ������Ƃ���ł�.

�����Ȃ��, ���͂�^�C�}�[�̘b�Ƃ������
[�^�X�N�X�P�W���[�����O](https://learn.microsoft.com/en-us/windows/win32/procthread/scheduling)
�̘b�ɂȂ��Ă���̂ł���,
�� TP �Ƃ��Ă͖ړI�͉ʂ����Ă��܂����̂�, ���������b�͌@�艺����ł��Ȃ�, ����ɂĊ���.


## �x��

[`timeBeginPeriod`](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod)
���Ă񂾕����^�C�}�[�����m�ɂȂ邩��Ƃ�����,
�����C�y�ɌĂԂ̂��l�����̂ł�.
CPU �g�p���������镪, �d�͂���������܂�.
<sub>( Microsoft �������Ɛߓd��~~������~~���R�ɋ����Ă��܂�. )</sub>

����
[Google Chrome �������Ȃ��� `timeBeginPeriod` ���Ă�ŏ���d�͂𑝂₵�Ă���](https://gigazine.net/news/20140715-google-chrome-laptop-battery/)
�Ƃ�����������܂���.
`timeBeginPeriod` ���Ă�łł��^�C�}�[���ׂ������ׂ��K�R��������̂��ǂ���,
�ĂԑO�ɂ悭�l����ׂ��ł��傤.
����, [�{ TP ����铮�@�ƂȂ����A�v��](../../../../Bulletin/) �ł�,
�u����A�����܂ł��闝�R�͂Ȃ��ȁB�v�Ɣ��f��, `timeBeginPeriod` �� ( �f�t�H���g�ł� ) �Ă΂Ȃ������Ƃ��܂���.


## ����

`timeBeginPeriod` �������Ă��Ă�, ����\�� 1[ms] �ɋl�߂邾���Ȃ̂�,
���̏����Ɋ��荞�܂��m�����f�t�H���g�̏�Ԃ�� `1/15.625` �Ɂg�������h�ɉ߂��܂���.

����R�A��ŉ��� CPU ���Ԃ�h��ɋ򂢂Ԃ��킪�܂܂ȃv���Z�X��������,
������̃^�C�}�[�����̓u���邱�ƂɂȂ�ł��傤.
���������v���Z�X�̑�\�Ⴊ �gMicrosoft Edge�h �ł��傤��.
�A�����ЂƂN�������, Task Manager �ɂ͈ȉ��̂悤�ȃv���Z�X�ƃX���b�h�̒c�̂��\������܂�.

<img src="MSEdge.png">

���ꂾ���̃X���b�h���N�����ꂽ��, �R�A���������Ă��S�āg�����h����Ă��܂��܂�.
�^�C�}�[�����͕K������R�A��� Microsoft Edge �� CPU ���Ԃ̒D��������������H�ڂɂȂ�܂�.

�Ƃ����킯��, ���̌v�������{�����ۂɐS�|�����̂�

* Microsoft Edge �Ȃǂ̎��Ȓ��ȃA�v�����N�����Ȃ�.
* Visual Studio �͕��Ă���.
* �e�X�g���̓}�E�X���������Ȃ�. ( Mouse Move �� message ���������ăm�C�Y�ɂȂ�̂� )

�Ƃ����������ł�.
���̑��ɁuStartup �Ɏd����ł���A�v�����N�����Ȃ��悤�ɂ���v�Ȃǂ̐������l�����܂���,
���܂蕁�i�̎g�����ƈقȂ�󋵂Ōv�����Ă����A���e�B�[�Ɍ����܂�.
����ȁu�e�X�g�̑O�������Ђ��̕׋�������v�悤�Ȃ��Ƃ͂��Ƃ͂���,
���i�̂���̂܂܂����߂�̂��ǂ��̂ł͂Ȃ��ł��傤��.
( �t��, �����ł��邾����������, ���i�ǂꂾ���d���v���Z�X�𑖂点�Ă��邩�����Ă݂�, �Ƃ����l����������܂���. )

�{�A�v���̃\�[�X�𗎂Ƃ���, �f�o�b�O���Ă݂悤�Ǝv��������,
�ŏI�m�F�͕K�� Release Build �� .exe �𒼐ڋN������悤�S�|���Ă����Ă�������.
Visual Studio �̒��ł̎��s�͔����܂��傤. ����ǂ��납 Visual Studio �����邾���ł��e��������܂�.
Visual Studio �͕��Ă����܂��傤.
Microsoft Edge �� MSDN �̃y�[�W���J�����܂�
Visual Studio �̒��Ńf�o�b�O����c�c�ȂǂƂ������u�����̂����v�����Ă����,
���m�Ȍv���ɂ͂Ȃ�܂���.

���̏��, ���x�����������Ōv�����J��Ԃ�,

* �����΂�������Ă���.
* CPU �g�p���������΂�Ⴂ.

���̗p���ׂ��v�����ʂł�.
�������Â�������, CPU �g�p�������ɏ�U�ꂵ�Ă���悤�Ȍv�����ʂɂ�,
�Ȃɂ������m�C�Y�����������Ƃ��l����������.
���̊��o�����ނɂ�, ���ǉ��񂩃e�X�g���J��Ԃ����ƂɂȂ�Ǝv���܂�.


## ��s

������, �f�t�H���g�̕���\�� `15.625[ms]` ( 1000/64[s] ) �Ƃ������r���[�Ȑ���, �ǂ��ɂ��Ȃ�Ȃ�������ł�����.
`10[ms]` �� `20[ms]` �ɂ��Ă�����, ����������ƋC�����I�ɂ�������ł�����ł����c�c.
��, ���� `15.625[ms]` �Ƃ���������, Microsoft �͐ϋɓI�ɐ������Ă��Ȃ��悤�Ɋ����܂�
( �܂�, �����̏�ł́u�n�[�h�E�F�A�ˑ��v������Ȃ�ł��傤���� ).

�������Ő��Ԃł� `15[ms]` �Ƃ� `��16[ms]` �Ƃ� `15.6[ms]` �Ƃ���G�c�Ȑ����Ō����Ă��邱�Ƃ������Ȃ��Ă���,
�킯�킩��Ȃ����̂�, �]�v�킯�킩��Ȃ��Ȃ��Ă��܂�.
Microsoft �� `15.625` �Ƃ��������������o���ĉ�����������Ă���̂���, 
[����](https://learn.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps)
��
[�R�R](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-kequerysystemtimeprecise)
���炢����Ȃ��ł�����?


## ���_

Windows �Ő��x�̍����^�C�}�[������������������, ���[�U�[������Ȃ�̓w�͂�����Ƃ������Ƃ炵���ł�.


<p style="text-align: right;">
In-house Tool / �ƒ� �O
</p>
