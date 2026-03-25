using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class hajjealesselaaaaaaaaopet : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Artikal_Korisnik_KorisnikId",
                table: "Artikal");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_Artikal_ArtikalID",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_Korisnik_KorisnikID",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_Korisnik_KurirskaSluzbaID",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Notifikacija_Korisnik_KorisnikId",
                table: "Notifikacija");

            migrationBuilder.DropForeignKey(
                name: "FK_Pracenje_Korisnik_PraceniKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropForeignKey(
                name: "FK_Pracenje_Korisnik_PratilacKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropTable(
                name: "Korisnik");

            migrationBuilder.DropIndex(
                name: "IX_Pracenje_PraceniKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropIndex(
                name: "IX_Pracenje_PratilacKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropIndex(
                name: "IX_Notifikacija_KorisnikId",
                table: "Notifikacija");

            migrationBuilder.DropIndex(
                name: "IX_Narudzba_KorisnikID",
                table: "Narudzba");

            migrationBuilder.DropIndex(
                name: "IX_Narudzba_KurirskaSluzbaID",
                table: "Narudzba");

            migrationBuilder.DropIndex(
                name: "IX_Artikal_KorisnikId",
                table: "Artikal");

            migrationBuilder.DropColumn(
                name: "PraceniID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PraceniKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PratilacID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PratilacKorisnikID",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "KorisnikId",
                table: "Notifikacija");

            migrationBuilder.DropColumn(
                name: "KorisnikID",
                table: "Narudzba");

            migrationBuilder.DropColumn(
                name: "KurirskaSluzbaID",
                table: "Narudzba");

            migrationBuilder.DropColumn(
                name: "KorisnikId",
                table: "Artikal");

            migrationBuilder.AddColumn<string>(
                name: "PraceniIDId",
                table: "Pracenje",
                type: "nvarchar(450)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "PratilacIDId",
                table: "Pracenje",
                type: "nvarchar(450)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "KorisnikIdId",
                table: "Notifikacija",
                type: "nvarchar(450)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "KorisnikIdId",
                table: "Narudzba",
                type: "nvarchar(450)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "KurirskaSluzbaIDId",
                table: "Narudzba",
                type: "nvarchar(450)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "Adresa",
                table: "AspNetUsers",
                type: "nvarchar(max)",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<string>(
                name: "BrojTelefona",
                table: "AspNetUsers",
                type: "nvarchar(max)",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<DateTime>(
                name: "DatumRegistracije",
                table: "AspNetUsers",
                type: "datetime2",
                nullable: false,
                defaultValue: new DateTime(1, 1, 1, 0, 0, 0, 0, DateTimeKind.Unspecified));

            migrationBuilder.AddColumn<string>(
                name: "EmailAdresa",
                table: "AspNetUsers",
                type: "nvarchar(max)",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<string>(
                name: "Ime",
                table: "AspNetUsers",
                type: "nvarchar(max)",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<string>(
                name: "KurirskaSluzba",
                table: "AspNetUsers",
                type: "nvarchar(max)",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "Prezime",
                table: "AspNetUsers",
                type: "nvarchar(max)",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<int>(
                name: "Uloga",
                table: "AspNetUsers",
                type: "int",
                nullable: false,
                defaultValue: 0);

            migrationBuilder.AddColumn<string>(
                name: "KorisnikIdId",
                table: "Artikal",
                type: "nvarchar(450)",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_Pracenje_PraceniIDId",
                table: "Pracenje",
                column: "PraceniIDId");

            migrationBuilder.CreateIndex(
                name: "IX_Pracenje_PratilacIDId",
                table: "Pracenje",
                column: "PratilacIDId");

            migrationBuilder.CreateIndex(
                name: "IX_Notifikacija_KorisnikIdId",
                table: "Notifikacija",
                column: "KorisnikIdId");

            migrationBuilder.CreateIndex(
                name: "IX_Narudzba_KorisnikIdId",
                table: "Narudzba",
                column: "KorisnikIdId");

            migrationBuilder.CreateIndex(
                name: "IX_Narudzba_KurirskaSluzbaIDId",
                table: "Narudzba",
                column: "KurirskaSluzbaIDId");

            migrationBuilder.CreateIndex(
                name: "IX_Artikal_KorisnikIdId",
                table: "Artikal",
                column: "KorisnikIdId");

            migrationBuilder.AddForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikIdId",
                table: "Artikal",
                column: "KorisnikIdId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_Artikal_ArtikalID",
                table: "Narudzba",
                column: "ArtikalID",
                principalTable: "Artikal",
                principalColumn: "ID");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikIdId",
                table: "Narudzba",
                column: "KorisnikIdId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIDId",
                table: "Narudzba",
                column: "KurirskaSluzbaIDId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Notifikacija_AspNetUsers_KorisnikIdId",
                table: "Notifikacija",
                column: "KorisnikIdId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Pracenje_AspNetUsers_PraceniIDId",
                table: "Pracenje",
                column: "PraceniIDId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Pracenje_AspNetUsers_PratilacIDId",
                table: "Pracenje",
                column: "PratilacIDId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Artikal_AspNetUsers_KorisnikIdId",
                table: "Artikal");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_Artikal_ArtikalID",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikIdId",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIDId",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Notifikacija_AspNetUsers_KorisnikIdId",
                table: "Notifikacija");

            migrationBuilder.DropForeignKey(
                name: "FK_Pracenje_AspNetUsers_PraceniIDId",
                table: "Pracenje");

            migrationBuilder.DropForeignKey(
                name: "FK_Pracenje_AspNetUsers_PratilacIDId",
                table: "Pracenje");

            migrationBuilder.DropIndex(
                name: "IX_Pracenje_PraceniIDId",
                table: "Pracenje");

            migrationBuilder.DropIndex(
                name: "IX_Pracenje_PratilacIDId",
                table: "Pracenje");

            migrationBuilder.DropIndex(
                name: "IX_Notifikacija_KorisnikIdId",
                table: "Notifikacija");

            migrationBuilder.DropIndex(
                name: "IX_Narudzba_KorisnikIdId",
                table: "Narudzba");

            migrationBuilder.DropIndex(
                name: "IX_Narudzba_KurirskaSluzbaIDId",
                table: "Narudzba");

            migrationBuilder.DropIndex(
                name: "IX_Artikal_KorisnikIdId",
                table: "Artikal");

            migrationBuilder.DropColumn(
                name: "PraceniIDId",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "PratilacIDId",
                table: "Pracenje");

            migrationBuilder.DropColumn(
                name: "KorisnikIdId",
                table: "Notifikacija");

            migrationBuilder.DropColumn(
                name: "KorisnikIdId",
                table: "Narudzba");

            migrationBuilder.DropColumn(
                name: "KurirskaSluzbaIDId",
                table: "Narudzba");

            migrationBuilder.DropColumn(
                name: "Adresa",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "BrojTelefona",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "DatumRegistracije",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "EmailAdresa",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "Ime",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "KurirskaSluzba",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "Prezime",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "Uloga",
                table: "AspNetUsers");

            migrationBuilder.DropColumn(
                name: "KorisnikIdId",
                table: "Artikal");

            migrationBuilder.AddColumn<int>(
                name: "PraceniID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "PraceniKorisnikID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "PratilacID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "PratilacKorisnikID",
                table: "Pracenje",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "KorisnikId",
                table: "Notifikacija",
                type: "int",
                nullable: false,
                defaultValue: 0);

            migrationBuilder.AddColumn<int>(
                name: "KorisnikID",
                table: "Narudzba",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "KurirskaSluzbaID",
                table: "Narudzba",
                type: "int",
                nullable: true);

            migrationBuilder.AddColumn<int>(
                name: "KorisnikId",
                table: "Artikal",
                type: "int",
                nullable: false,
                defaultValue: 0);

            migrationBuilder.CreateTable(
                name: "Korisnik",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1")
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Korisnik", x => x.ID);
                });

            migrationBuilder.CreateIndex(
                name: "IX_Pracenje_PraceniKorisnikID",
                table: "Pracenje",
                column: "PraceniKorisnikID");

            migrationBuilder.CreateIndex(
                name: "IX_Pracenje_PratilacKorisnikID",
                table: "Pracenje",
                column: "PratilacKorisnikID");

            migrationBuilder.CreateIndex(
                name: "IX_Notifikacija_KorisnikId",
                table: "Notifikacija",
                column: "KorisnikId");

            migrationBuilder.CreateIndex(
                name: "IX_Narudzba_KorisnikID",
                table: "Narudzba",
                column: "KorisnikID");

            migrationBuilder.CreateIndex(
                name: "IX_Narudzba_KurirskaSluzbaID",
                table: "Narudzba",
                column: "KurirskaSluzbaID");

            migrationBuilder.CreateIndex(
                name: "IX_Artikal_KorisnikId",
                table: "Artikal",
                column: "KorisnikId");

            migrationBuilder.AddForeignKey(
                name: "FK_Artikal_Korisnik_KorisnikId",
                table: "Artikal",
                column: "KorisnikId",
                principalTable: "Korisnik",
                principalColumn: "ID",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_Artikal_ArtikalID",
                table: "Narudzba",
                column: "ArtikalID",
                principalTable: "Artikal",
                principalColumn: "ID",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_Korisnik_KorisnikID",
                table: "Narudzba",
                column: "KorisnikID",
                principalTable: "Korisnik",
                principalColumn: "ID",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_Korisnik_KurirskaSluzbaID",
                table: "Narudzba",
                column: "KurirskaSluzbaID",
                principalTable: "Korisnik",
                principalColumn: "ID",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_Notifikacija_Korisnik_KorisnikId",
                table: "Notifikacija",
                column: "KorisnikId",
                principalTable: "Korisnik",
                principalColumn: "ID",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_Pracenje_Korisnik_PraceniKorisnikID",
                table: "Pracenje",
                column: "PraceniKorisnikID",
                principalTable: "Korisnik",
                principalColumn: "ID");

            migrationBuilder.AddForeignKey(
                name: "FK_Pracenje_Korisnik_PratilacKorisnikID",
                table: "Pracenje",
                column: "PratilacKorisnikID",
                principalTable: "Korisnik",
                principalColumn: "ID");
        }
    }
}
