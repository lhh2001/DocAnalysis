$wordApp = New-Object -comobject word.application

For($i = 0; $i -lt $args.Count; $i++)
{
    $dstPath = Resolve-Path $args[$i]
    $dstPath = $dstPath.ToString()
    $wordDoc = $wordApp.Documents.Open($dstPath)
    $dstPath = $dstPath -replace "\.docx", ""
    $dstPath = $dstPath -replace "\.doc", ""
    $wordDoc.SaveAs($dstPath + ".txt", 2)
    $wordDoc.Close()
}
$wordApp.Quit()
