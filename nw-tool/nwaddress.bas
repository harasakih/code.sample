Attribute VB_Name = "Module1"
Option Explicit

Function ipinnet(ip As String, net As String) As Boolean
    Dim ip_network As String
    Dim net_ip As String
    Dim net_net As String
    Dim net_network As String
    Dim lslash As Integer
    
    lslash = InStr(1, net, "/")
    net_ip = Left(net, lslash - 1)
    net_net = Mid(net, lslash + 1, Len(net))

    ip_network = ipwild2nwaddress(ip + "/" + net_net)
    net_network = ipwild2nwaddress(net_ip + "/" + net_net)
    
    If ip_network = net_network Then
        ipinnet = True
    Else
        ipinnet = False
    End If
End Function


Function ipwild2broadaddr(ipwild As String) As String
    Dim ipaddress As String
    Dim wildcardmask As String
    Dim subnet As String
    Dim lslash As Integer
    Dim sMaskl As String
    Dim ldot(3) As Integer
    Dim iip(4) As Integer
    Dim isubnet(4) As Integer
    Dim iwild(4) As Integer
    Dim irtn(4) As Integer
    Dim tmp As String
    Dim rtn(4)
    Dim i As Integer
    
    lslash = InStr(1, ipwild, "/")
    ipaddress = Left(ipwild, lslash - 1)
    wildcardmask = Mid(ipwild, lslash + 1, Len(ipwild))
    subnet = wild2subnetmask(wildcardmask)
    
    ldot(1) = InStr(1, ipaddress, ".")
    ldot(2) = InStr(ldot(1) + 1, ipaddress, ".")
    ldot(3) = InStr(ldot(2) + 1, ipaddress, ".")
    iip(1) = Left(ipaddress, ldot(1) - 1)
    iip(2) = Mid(ipaddress, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    iip(3) = Mid(ipaddress, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    iip(4) = Mid(ipaddress, ldot(3) + 1, Len(ipaddress))
    
    ldot(1) = InStr(1, subnet, ".")
    ldot(2) = InStr(ldot(1) + 1, subnet, ".")
    ldot(3) = InStr(ldot(2) + 1, subnet, ".")
    isubnet(1) = Left(subnet, ldot(1) - 1)
    isubnet(2) = Mid(subnet, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    isubnet(3) = Mid(subnet, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    isubnet(4) = Mid(subnet, ldot(3) + 1, Len(subnet))
    
    ldot(1) = InStr(1, wildcardmask, ".")
    ldot(2) = InStr(ldot(1) + 1, wildcardmask, ".")
    ldot(3) = InStr(ldot(2) + 1, wildcardmask, ".")
    iwild(1) = Left(wildcardmask, ldot(1) - 1)
    iwild(2) = Mid(wildcardmask, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    iwild(3) = Mid(wildcardmask, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    iwild(4) = Mid(wildcardmask, ldot(3) + 1, Len(subnet))
    
    
    For i = 1 To 4
        irtn(i) = (iip(i) And isubnet(i)) Or iwild(i)
        tmp = Str(irtn(i))
        rtn(i) = Mid(tmp, 2, Len(tmp) - 1)
    Next i
    ipwild2broadaddr = rtn(1) & "." & rtn(2) & "." & rtn(3) & "." & rtn(4)
End Function



Function ipwild2nwaddress(ipwild As String) As String
    Dim ipaddress As String
    Dim wildcardmask As String
    Dim subnet As String
    Dim lslash As Integer
    Dim sMaskl As String
    Dim ldot(3) As Integer
    Dim iip(4) As Integer
    Dim isubnet(4) As Integer
    Dim irtn(4) As Integer
    Dim tmp As String
    Dim rtn(4)
    Dim i As Integer
    
    lslash = InStr(1, ipwild, "/")
    ipaddress = Left(ipwild, lslash - 1)
    wildcardmask = Mid(ipwild, lslash + 1, Len(ipwild))
    subnet = wild2subnetmask(wildcardmask)
    
    ldot(1) = InStr(1, ipaddress, ".")
    ldot(2) = InStr(ldot(1) + 1, ipaddress, ".")
    ldot(3) = InStr(ldot(2) + 1, ipaddress, ".")
    iip(1) = Left(ipaddress, ldot(1) - 1)
    iip(2) = Mid(ipaddress, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    iip(3) = Mid(ipaddress, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    iip(4) = Mid(ipaddress, ldot(3) + 1, Len(ipaddress))
    
    ldot(1) = InStr(1, subnet, ".")
    ldot(2) = InStr(ldot(1) + 1, subnet, ".")
    ldot(3) = InStr(ldot(2) + 1, subnet, ".")
    isubnet(1) = Left(subnet, ldot(1) - 1)
    isubnet(2) = Mid(subnet, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    isubnet(3) = Mid(subnet, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    isubnet(4) = Mid(subnet, ldot(3) + 1, Len(subnet))
    
    For i = 1 To 4
        irtn(i) = iip(i) And isubnet(i)
        tmp = Str(irtn(i))
        rtn(i) = Mid(tmp, 2, Len(tmp) - 1)
    Next i
    ipwild2nwaddress = rtn(1) & "." & rtn(2) & "." & rtn(3) & "." & rtn(4)
End Function


Function wild2subnetmask(sWild As String) As String
    Dim oct(4) As String
    Dim ldot(3) As Integer
    Dim i As Integer
    Dim wild(4) As String
    Dim tmp As String
    Dim rtn
    
    ldot(1) = InStr(1, sWild, ".")
    ldot(2) = InStr(ldot(1) + 1, sWild, ".")
    ldot(3) = InStr(ldot(2) + 1, sWild, ".")
    
    oct(1) = Left(sWild, ldot(1) - 1)
    oct(2) = Mid(sWild, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    oct(3) = Mid(sWild, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    oct(4) = Mid(sWild, ldot(3) + 1, Len(sWild))

    For i = 1 To 4
        tmp = Str(255 - CInt(oct(i)))
        wild(i) = Mid(tmp, 2, Len(tmp) - 1)
    Next i
    wild2subnetmask = wild(1) & "." & wild(2) & "." & wild(3) & "." & wild(4)
End Function


Function ipwild2ipmaskl(sNetwork As String) As String
    Dim ipaddress As String
    Dim wildcardmask As String
    Dim lslash As Integer
    Dim sMaskl As String
    
    Dim rtn As String
    
    lslash = InStr(1, sNetwork, "/")
    ipaddress = Left(sNetwork, lslash - 1)
    wildcardmask = Mid(sNetwork, lslash + 1, Len(sNetwork))
    sMaskl = Str(wild2maskl(wildcardmask))
    sMaskl = Mid(sMaskl, 2, Len(sMaskl) - 1)
    
    rtn = ipaddress & "/" & sMaskl
    ipwild2ipmaskl = rtn
End Function

Function ipmaskl2ipwild(sNetwork As String) As String
    Dim ipaddress As String
    Dim sWild As String
    Dim lslash As Integer
    Dim sMaskl As String
    
    Dim rtn As String
    
    lslash = InStr(1, sNetwork, "/")
    ipaddress = Left(sNetwork, lslash - 1)
    sMaskl = Mid(sNetwork, lslash + 1, Len(sNetwork))
    sWild = maskl2wild(sMaskl)
    
    rtn = ipaddress & "/" & sWild
    ipmaskl2ipwild = rtn
End Function


Function Ip2Int(sIp As String) As Variant
    Dim oct(4) As String
    Dim ldot(3) As Integer
    Dim rtn
    
    ldot(1) = InStr(1, sIp, ".")
    ldot(2) = InStr(ldot(1) + 1, sIp, ".")
    ldot(3) = InStr(ldot(2) + 1, sIp, ".")
    
    oct(1) = Left(sIp, ldot(1) - 1)
    oct(2) = Mid(sIp, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    oct(3) = Mid(sIp, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    oct(4) = Mid(sIp, ldot(3) + 1, Len(sIp))
    
    rtn = ((oct(1) * 256 + oct(2)) * 256 + oct(3)) * 256 + oct(4)
    Ip2Int = rtn
End Function

Function wild2maskl(sWild As String) As Integer
    Dim oct(4) As String
    Dim ldot(3) As Integer
    Dim l  As Integer
    Dim rtn As Integer
    Dim i As Integer
    
    ldot(1) = InStr(1, sWild, ".")
    ldot(2) = InStr(ldot(1) + 1, sWild, ".")
    ldot(3) = InStr(ldot(2) + 1, sWild, ".")
    
    oct(1) = Left(sWild, ldot(1) - 1)
    oct(2) = Mid(sWild, ldot(1) + 1, ldot(2) - ldot(1) - 1)
    oct(3) = Mid(sWild, ldot(2) + 1, ldot(3) - ldot(2) - 1)
    oct(4) = Mid(sWild, ldot(3) + 1, Len(sWild))

    rtn = 0
    For i = 1 To 4
        l = -1
        Select Case CInt(oct(i))
        Case 0
            l = 0
        Case 1
            l = 1
        Case 3
            l = 2
        Case 7
            l = 3
        Case 15
            l = 4
        Case 31
            l = 5
        Case 63
            l = 6
        Case 127
            l = 7
        Case 255
            l = 8
        End Select
        
        If l < 0 Then
            wild2maskl = -1
            Exit Function
        Else
            rtn = rtn + l
        End If
    Next i
    
    wild2maskl = 32 - rtn
End Function

Function maskl2wild(sMaskl As Variant) As String
    Dim wild(4) As String
    Dim iMaskl As Integer
    Dim n As Integer
    Dim m As Integer
    Dim i As Integer
    
    iMaskl = CLng(sMaskl)
    n = Int(iMaskl / 8)
    m = iMaskl Mod 8
    Select Case n
    Case 4
    ' maskl = 32
        wild(1) = "0"
        wild(2) = "0"
        wild(3) = "0"
        wild(4) = "0"
    Case 3
    ' maskl = 31 - 24
        wild(1) = "0"
        wild(2) = "0"
        wild(3) = "0"
        wild(4) = maskl2wildoct(m)
    Case 2
    ' maskl = 23 - 16
        wild(1) = "0"
        wild(2) = "0"
        wild(3) = maskl2wildoct(m)
        wild(4) = "255"
    Case 1
    ' maskl = 15 - 8
        wild(1) = "0"
        wild(2) = maskl2wildoct(m)
        wild(3) = "255"
        wild(4) = "255"
    Case 0
    ' maskl = 7 - 0
        wild(1) = maskl2wildoct(m)
        wild(2) = "255"
        wild(3) = "255"
        wild(4) = "255"
    Case Else
        wild(1) = "0"
        wild(2) = "0"
        wild(3) = "0"
        wild(4) = "0"
    End Select
    
    maskl2wild = wild(1) & "." & wild(2) & "." & wild(3) & "." & wild(4)
    
End Function

Function maskl2wildoct(iMaskl As Variant) As String

    Select Case CInt(iMaskl)
    Case 7
        maskl2wildoct = "1"
    Case 6
        maskl2wildoct = "3"
    Case 5
        maskl2wildoct = "7"
    Case 4
        maskl2wildoct = "15"
    Case 3
        maskl2wildoct = "31"
    Case 2
        maskl2wildoct = "63"
    Case 1
        maskl2wildoct = "127"
    Case 0
        maskl2wildoct = "255"
    End Select
End Function
