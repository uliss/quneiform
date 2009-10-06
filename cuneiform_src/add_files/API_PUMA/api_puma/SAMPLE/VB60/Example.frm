VERSION 5.00
Begin VB.Form Test 
   Caption         =   "Form1"
   ClientHeight    =   2565
   ClientLeft      =   45
   ClientTop       =   270
   ClientWidth     =   3750
   LinkTopic       =   "Form1"
   ScaleHeight     =   2565
   ScaleWidth      =   3750
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton BCLIPBOARD 
      Caption         =   "Распознать из CLIPBOARD"
      Height          =   252
      Left            =   120
      TabIndex        =   0
      Top             =   2040
      Width           =   3492
   End
   Begin VB.Label StepPerc 
      Height          =   252
      Left            =   3000
      TabIndex        =   2
      Top             =   1680
      Width           =   612
   End
   Begin VB.Label StepName 
      Height          =   252
      Left            =   120
      TabIndex        =   1
      Top             =   1680
      Width           =   2772
   End
End
Attribute VB_Name = "Test"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

'Dim Tiger As Object
Dim WithEvents Tiger As Recognition
Attribute Tiger.VB_VarHelpID = -1
Dim rc As Integer


Private Sub BCLIPBOARD_Click()
    rc = Tiger.RecogClipboard()
End Sub

Private Sub Form_Load()
    Set Tiger = CreateObject("Cognitive.Puma")
    rc = Tiger.Load()
    StepName.Visible = True
    StepPerc.Visible = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Tiger.Unload
End Sub

Private Function Tiger_ProgressFinish() As Long
    StepName.Caption = ""
    StepPerc.Caption = ""
    StepPerc.Refresh
    StepName.Refresh
    Tiger_ProgressFinish = 1
End Function

Private Function Tiger_ProgressStart() As Long
    StepName.Caption = ""
    StepPerc.Caption = ""
    StepPerc.Refresh
    StepName.Refresh
    Tiger_ProgressStart = 1
End Function


Private Function Tiger_ProgressStep(ByVal lStep As Long, ByVal btName As String, ByVal lPercent As Long) As Long
    StepName.Caption = btName
    StepPerc.Caption = lPercent
    StepPerc.Refresh
    StepName.Refresh
    Tiger_ProgressStep = 1
End Function

