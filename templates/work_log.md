<%*tR = "";-%>
## Log
- [ ]
## Next
- [ ]

<%*
const filename = tp.file.title; // e.g., "06_09_2025"
const [year, month, day] = filename.split("-").map(Number);
const date = new Date(year, month - 1, day);
const dayOfWeek = date.getDay(); // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
const prevDate = new Date(year, month - 1, day);
const nextDate = new Date(year, month - 1, day);

if ( dayOfWeek == 1 ) {
prevDate.setDate(date.getDate() - 3);
nextDate.setDate(date.getDate() + 1);
} else if ( dayOfWeek == 5 ) {
prevDate.setDate(date.getDate() - 1);
nextDate.setDate(date.getDate() + 3);
} else {
prevDate.setDate(date.getDate() - 1);
nextDate.setDate(date.getDate() + 1);
}

const pad = n => n.toString().padStart(2, '0');
const prev = `${prevDate.getFullYear()}-${pad(prevDate.getMonth() + 1)}-${pad(prevDate.getDate())}`;
const next = `${nextDate.getFullYear()}-${pad(nextDate.getMonth() + 1)}-${pad(nextDate.getDate())}`;
tR += `[[${prev}|prev]] [[${next}|next]]`;
-%>

