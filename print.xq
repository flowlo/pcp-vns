declare namespace env = 'http://www.w3.org/2003/05/soap-envelope';
declare namespace m = 'http://www.w3.org/2005/10/markup-validator';

let $doc := doc('/tmp/validation.xml')
let $response := $doc/env:Envelope/env:Body/m:markupvalidationresponse
let $validity := $response/m:validity/text()
let $file :=  $response/m:uri/text()
let $newline := '&#10;'

return
(
	if (compare($validity, 'true')) then
		concat(
			string-join(
				(for $error in $response/m:errors/m:errorlist/m:error
					return concat($file, ':', $error/m:line/text(), ':', $error/m:col/text(), ' ', $error/m:message/text())
				),
				$newline
			),
			string-join(
				(for $warning in $response/m:warnings/m:warninglist/m:error
					return concat($file, ':', $warning/m:line/text(), ':', $warning/m:col/text(), ' ', $warning/m:message/text())
				),
				$newline
			)
		)
	else
		concat(
			$file,
			" is valid"
		)
)
